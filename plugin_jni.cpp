#include <jni.h>
#include <sched.h>

#include <span>
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

uint getPackageIdentifier(std::span<char> packageNameId)
{
    auto thEnv{saglobal::g_gameEnv};
    auto gtasaClass{thEnv->FindClass("android/app/Application")};
    jclass threadClass{thEnv->FindClass("android/app/ActivityThread")};
    
    jmethodID getNameMid{thEnv->GetMethodID(gtasaClass, "getPackageName", "()Ljava/lang/String;")};
    jmethodID threadMid{thEnv->GetStaticMethodID(threadClass,
        "currentActivityThread", "()Landroid/app/ActivityThread;")};

    jobject activityThreadObj{thEnv->CallStaticObjectMethod(threadClass, threadMid)};
    jmethodID getGTASAMid{thEnv->GetMethodID(threadClass, "getApplication", "()Landroid/app/Application;")};
    jobject GTASAobj{thEnv->CallObjectMethod(activityThreadObj, getGTASAMid)};

    auto GTASAid{static_cast<jstring>(thEnv->CallObjectMethod(GTASAobj, getNameMid))};

    const char* redable{thEnv->GetStringUTFChars(GTASAid, nullptr)};
    auto packStrLen{strlen(redable)};

    std::strncpy(packageNameId.data(), redable, packageNameId.size());

    thEnv->ReleaseStringUTFChars(GTASAid, redable);
    thEnv->DeleteLocalRef(GTASAid);
    thEnv->DeleteLocalRef(GTASAobj);
    thEnv->DeleteLocalRef(activityThreadObj);
    thEnv->DeleteLocalRef(threadClass);
    thEnv->DeleteLocalRef(gtasaClass);

    return packStrLen;
}

extern "C" jint JNI_OnLoad(JavaVM* vm, [[maybe_unused]] void* reserved)
{
    salog::print(ANDROID_LOG_INFO, "SAMobile has loaded, build date: " __DATE__ " " __TIME__);
    salog::coutFmt(ANDROID_LOG_INFO, "Loaded thread id {} in core {}",
        std::this_thread::get_id(), sched_getcpu());

    const jint useVersion{JNI_VERSION_1_6};

    if (vm->GetEnv(reinterpret_cast<void**>(&saglobal::g_gameEnv), useVersion) != JNI_OK) {
        salog::print(ANDROID_LOG_ERROR, "Can't get the JNI interface!");
        vm->DetachCurrentThread();
    }
    std::array<char, 40> gtasaPackage;
    if (getPackageIdentifier(gtasaPackage)) {
      if (gtasaPackage.data() != std::string_view("com.rockstargames.gtasa"))
        std::terminate();
    }

    JavaVMAttachArgs auxThread{.version = useVersion, .name = "SAMobileMain"};
    vm->AttachCurrentThread(&saglobal::g_gameEnv, &auxThread);
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

