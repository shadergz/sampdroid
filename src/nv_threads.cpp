#include <mutex>

#include <core/log_client.h>
#include <nv_threads.h>
void (*g_NVThreadSpawnProc)(uintptr_t x0);

// Count of created threads
static uint8_t thCount{};
static std::mutex nvLock{};

void NVThreadSpawnProc(uintptr_t x0)
{
    std::unique_lock<std::mutex> nvWaitLocker(nvLock);
    useriDsp("Threads: on (NVThreadSpawnProc: %u)", thCount);
    thCount++;

    nvWaitLocker.unlock();

    g_NVThreadSpawnProc(x0);
}
