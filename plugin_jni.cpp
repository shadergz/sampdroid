#include <sched.h>
#include <thread>

#include <cstdlib>

#include <log_client.h>
#include <linux_hierarchy.h>
#include <patches_level.h>
#include <texture_runtime.h>

#include <ui/user_graphics.h>
#include <jvm_helper.h>
#include <main_thread.h>

namespace saglobal {
    // This env is specific by the calling thread and shouldn't be shared
    JNIEnv* g_gameEnv;
    jobject g_gtaSA;
    // Game engine, base address
    uintptr_t g_gameAddr;
    // OpenAL's address space, we will use later to direct produce sound wout contact the game engine
    // will be useful when a sound overlay is needed
    uintptr_t g_audioBackend;

    extern AArch64Patcher* g_patcherMicro;
    extern UiClientUser* g_playerUi;

    extern std::atomic<bool> g_clientHasInitiliazed;
}

namespace saclient {
    extern pthread_cond_t g_multCond;
    extern pthread_mutex_t g_multExclusive;
}

extern "C" void JNI_OnUnload([[maybe_unused]] JavaVM* vm, [[maybe_unused]] void* reserved)
{
    using namespace saglobal;
    salog::print(salog::LogId::Info, "Unload all used resources");

    if (g_playerUi)
        delete g_playerUi;
    if (g_patcherMicro)
        delete g_patcherMicro;

    if (g_textureDatabase)
        delete g_textureDatabase;

    if (g_clientHasInitiliazed) {

    }
}

extern "C" JNIEXPORT void JNICALL Java_com_rockstargames_gtasa_GTASA_JVMSaMobileReady(
    [[maybe_unused]] JNIEnv* env, jobject gtaClass) 
{
    saglobal::g_gtaSA = gtaClass;
    salog::print(salog::LogId::Info, "JVMSaMobileReady() has been called from JVM");

    pthread_mutex_lock(&saclient::g_multExclusive);
    pthread_cond_broadcast(&saclient::g_multCond);
    pthread_mutex_unlock(&saclient::g_multExclusive);

    // This thread should fall to the end, before game itself starts
}

extern "C" jint JNI_OnLoad(JavaVM* vm, [[maybe_unused]] void* reserved)
{
    using namespace saglobal;

    salog::print(salog::LogId::Info, "SAMobile has loaded, build date: " __DATE__ " " __TIME__);
    salog::coutFmt(salog::LogId::Info, "Loaded by thread id {} in core {}",
        std::this_thread::get_id(), sched_getcpu());

    const jint useVersion{JNI_VERSION_1_6};

    if (vm->GetEnv(reinterpret_cast<void**>(&g_gameEnv), useVersion) != JNI_OK) {
        salog::print(salog::LogId::Error, "Can't get the JNI interface!");
        vm->DetachCurrentThread();
        return JNI_ERR;
    }
    
    std::array<char, 40> gtasaPackage;
    if (sajvm::getPackageIdentifier(gtasaPackage)) {
        if (gtasaPackage.data() != std::string_view("com.rockstargames.gtasa"))
            std::terminate();
    }

    JavaVMAttachArgs attachThread{.version = useVersion, .name = "saclient"};
    vm->AttachCurrentThread(&g_gameEnv, &attachThread);
    // Fetches in memory GTASA base library address (where exatcly JVM has loaded the game engine)
    g_gameAddr = safs::getLibrary("libGTASA.so");
    g_audioBackend = safs::getLibrary("libOpenAL64.so");

    SALOG_ASSERT(g_gameAddr && g_audioBackend, "Can't found a valid address space of GTASA and/or OpenAL, "
        "SAMobile is being halted now :[");
    salog::printFormat(salog::LogId::Info, "Native libraries base region address found in:\n"
        "1. (GTASA) (%#lx)\n2. (OpenAL64) (%#lx)", g_gameAddr, g_audioBackend);

    // Applying patches and hooking some methods
    sapatch::applyOnGame();
    
    pthread_mutex_init(&saclient::g_multExclusive, nullptr);
    pthread_cond_init(&saclient::g_multCond, nullptr);

    pthread_t clientThread;
    pthread_create(&clientThread, nullptr, saclient::enterMainLoop, nullptr);

    return useVersion;
}

