#include <settings.h>
#include <outside.h>

#include <mutex>

void (*NVThreadSpawnProc)(uintptr_t x0);

// count of created threads
uint16_t g_th_count{};
std::mutex g_NVlock{};

void NVThreadSpawnProc_HOOK(uintptr_t x0)
{
    std::unique_lock<std::mutex> unique(g_NVlock);
    mtmprintf(ANDROID_LOG_WARN, "HOOKED: on (NVThreadSpawnProc: %#u)", g_th_count);
    g_th_count++;

    unique.unlock();

    NVThreadSpawnProc(x0);
}