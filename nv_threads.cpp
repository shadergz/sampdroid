#include <nv_threads.h>
#include <outside.h>

#include <mutex>

namespace Mt4Global {
void (*g_NVThreadSpawnProc)(uintptr_t x0);
}

// Count of created threads
static uint16_t thCount{};
static std::mutex NVLock{};

namespace Mt4Mimic {
void NVThreadSpawnProc(uintptr_t x0)
{
    std::unique_lock<std::mutex> unique(NVLock);
    Mt4Log::printflike(ANDROID_LOG_WARN, "NVThreadHook: on (NVThreadSpawnProc: %u)", thCount);
    thCount++;

    unique.unlock();
    
    Mt4Global::g_NVThreadSpawnProc(x0);
}

}
