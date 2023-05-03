#include "texture_runtime.h"
#include <jni.h>
#include <sched.h>
#include <cstdlib>

#include <outside.h>
#include <linux_hierarchy.h>
#include <patches_level.h>

// This env is specific by the calling thread and shouldn't be shared

namespace Mt4Global {
JNIEnv* g_gameEnv;
uintptr_t g_gameAddr;

extern AArch64_Patcher* g_patcherMicro;
}

namespace JNIOrigin {
using namespace Mt4Global;

extern "C" void JNI_OnUnload([[maybe_unused]] JavaVM *vm, [[maybe_unused]] void *reserved) 
{
    Mt4Log::putslike(ANDROID_LOG_INFO, "Unload all resources used");

    if (!g_patcherMicro) 
        delete g_patcherMicro;
    
    if (g_textureDatabase)
        delete g_textureDatabase;
}

extern "C" jint JNI_OnLoad(JavaVM *vm, [[maybe_unused]] void *reserved)
{
    Mt4Log::putslike(ANDROID_LOG_INFO, "MTM has started, build date: " __DATE__ " " __TIME__);    
    Mt4Log::coutlike(ANDROID_LOG_INFO, "Loaded thread id {} in core {}", 
        std::this_thread::get_id(), sched_getcpu());
    
    const jint use_version{JNI_VERSION_1_6};

    if (vm->GetEnv((void**)(&g_gameEnv), use_version) != JNI_OK) {
        Mt4Log::putslike(ANDROID_LOG_ERROR, "Can't get the JNI interface!");
        vm->DetachCurrentThread();
    }
    // Getting the in memory shared object address space!
    g_gameAddr = Mt4Fs::getLibrary("libGTASA.so");
    if (!g_gameAddr) {
        Mt4Log::putslike(ANDROID_LOG_ERROR, "Can't locate libGTASA.so, MT is halted!");
        return -1;
    }

    Mt4Log::printflike(ANDROID_LOG_INFO, "(libGTASA.so) base image found in address space: (%#lx)", 
        (void*)(g_gameAddr));
    
    // Applying patches and hooking some methods
    Mt4Patches::applyOnGame();
    
    return use_version;
}

}

