#include <settings.h>
#include <outside.h>

#include <mutex>

void (*NVThreadSpawnProc)(uintptr_t x0);

// count of created threads
uint16_t g_th_count{};
std::mutex g_NVlock{};

void NVThreadSpawnProc_HOOK(uintptr_t x0)
{
    std::lock_guard<std::mutex> loguard(g_NVlock);
    mtmcout(ANDROID_LOG_INFO, "On (NVThreadSpawnProc: {})", g_th_count);
    g_th_count++;

    NVThreadSpawnProc(x0);
}