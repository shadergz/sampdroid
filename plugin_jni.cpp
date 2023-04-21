#include <jni.h>

#include <outside.h>
#include <linux_hierarchy.h>
#include <patches_level.h>

// This env is specific by the calling thread and shouldn't be shared
JNIEnv* g_gameEnv;
uintptr_t g_gameAddr;

extern AArch64_Patcher* patcher_micro;

void JNI_OnUnload([[maybe_unused]] JavaVM *vm, [[maybe_unused]] void *reserved) 
{
    mtmputs(ANDROID_LOG_INFO, "Unload all resources used");

    if (!patcher_micro) 
        delete patcher_micro;
}

jint JNI_OnLoad(JavaVM *vm, [[maybe_unused]] void *reserved) {
    mtmputs(ANDROID_LOG_INFO, "MTM has started, build date: " __DATE__ " " __TIME__);
    const jint useVersion{JNI_VERSION_1_6};

    if (vm->GetEnv((void**)(&g_gameEnv), useVersion) != JNI_OK) {
        mtmputs(ANDROID_LOG_ERROR, "Can't get the JNI interface!");
        vm->DetachCurrentThread();
    }
    // Getting the in memory shared object address space!
    g_gameAddr = fhsGetLibrary("libGTASA.so");
    
    if (!g_gameAddr) {
        mtmputs(ANDROID_LOG_ERROR, "Can't locate libGTASA.so, MT is halted!");
        return -1;
    }

    mtmprintf(ANDROID_LOG_INFO, "libGTASA base image found in address space: (%#lx)", 
        reinterpret_cast<void*>(g_gameAddr));
    
    applyGlobalPatches();
    return useVersion;
}