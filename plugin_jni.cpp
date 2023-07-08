
#include <jni.h>
#include <sched.h>

#include <thread>

#include <cstdlib>

#include <log_client.h>
#include <linux_hierarchy.h>
#include <patches_level.h>
#include <texture_runtime.h>

// This env is specific by the calling thread and shouldn't be shared

namespace saglobal {
    JNIEnv* g_gameEnv;
    uintptr_t g_gameAddr;

    extern AArch64Patcher* g_patcherMicro;
}

extern "C" void JNI_OnUnload([[maybe_unused]] JavaVM* vm, [[maybe_unused]] void* reserved)
{
    salog::print(ANDROID_LOG_INFO, "Unload all resources used");

    if (!saglobal::g_patcherMicro)
        delete saglobal::g_patcherMicro;

    if (saglobal::g_textureDatabase)
        delete saglobal::g_textureDatabase;
}

extern "C" jint JNI_OnLoad(JavaVM* vm, [[maybe_unused]] void* reserved)
{
    salog::print(ANDROID_LOG_INFO, "SAMobile has started, build date: " __DATE__ " " __TIME__);
    salog::coutFmt(ANDROID_LOG_INFO, "Loaded thread id {} in core {}",
        std::this_thread::get_id(), sched_getcpu());

    const jint useVersion{JNI_VERSION_1_6};

    if (vm->GetEnv(reinterpret_cast<void**>(&saglobal::g_gameEnv), useVersion) != JNI_OK) {
        salog::print(ANDROID_LOG_ERROR, "Can't get the JNI interface!");
        vm->DetachCurrentThread();
    }
    // Getting the in memory shared object address space!
    saglobal::g_gameAddr = safs::getLibrary("libGTASA.so");
    if (!saglobal::g_gameAddr) {
        salog::print(ANDROID_LOG_ERROR, "Can't find libGTASA.so, SAMobile is being halted!");
        return -1;
    }

    salog::printFormat(ANDROID_LOG_INFO, "(libGTASA.so) base image in address "
        "region: (%#lx)", saglobal::g_gameAddr);

    // Applying patches and hooking some methods
    sapatch::applyOnGame();

    return useVersion;
}

