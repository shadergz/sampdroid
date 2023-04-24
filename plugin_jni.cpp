#include <jni.h>
#include <sched.h>
#include <stdlib.h>

#include <outside.h>
#include <linux_hierarchy.h>
#include <patches_level.h>

// this env is specific by the calling thread and shouldn't be shared
JNIEnv* g_game_env;
uintptr_t g_game_addr;

extern AArch64_Patcher* g_patcher_micro;

void JNI_OnUnload([[maybe_unused]] JavaVM *vm, [[maybe_unused]] void *reserved) 
{
    mtmputs(ANDROID_LOG_INFO, "unload all resources used");

    if (!g_patcher_micro) 
        delete g_patcher_micro;
}

jint JNI_OnLoad(JavaVM *vm, [[maybe_unused]] void *reserved) {
    mtmputs(ANDROID_LOG_INFO, "MTM has started, build date: " __DATE__ " " __TIME__);    
    mtmcout(ANDROID_LOG_INFO, "loaded thread id {} in core {}", 
        std::this_thread::get_id(), sched_getcpu());
    
    const jint use_version{JNI_VERSION_1_6};

    if (vm->GetEnv((void**)(&g_game_env), use_version) != JNI_OK) {
        mtmputs(ANDROID_LOG_ERROR, "can't get the JNI interface!");
        vm->DetachCurrentThread();
    }
    // getting the in memory shared object address space!
    g_game_addr = fhsGetLibrary("libGTASA.so");
    if (!g_game_addr) {
        mtmputs(ANDROID_LOG_ERROR, "can't locate libGTASA.so, MT is halted!");
        return -1;
    }

    mtmprintf(ANDROID_LOG_INFO, "libGTASA.so base image found in address space: (%#lx)", (void*)(g_game_addr));
    applyOnGamePatches();

    
    return use_version;
}