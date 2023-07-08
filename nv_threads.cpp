
#include <nv_threads.h>
#include <log_client.h>

#include <mutex>

namespace saglobal {
    void (*g_NVThreadSpawnProc)(uintptr_t x0);
}

// Count of created threads
static uint16_t thCount{};
static std::mutex nvLock{};

namespace samimic {
    void NVThreadSpawnProc(uintptr_t x0)
    {
        std::unique_lock<std::mutex> nvWaitLocker(nvLock);
        salog::printFormat(ANDROID_LOG_WARN, "NVThreadHook: on (NVThreadSpawnProc: %u)", thCount);
        thCount++;

        nvWaitLocker.unlock();

        saglobal::g_NVThreadSpawnProc(x0);
    }
}
