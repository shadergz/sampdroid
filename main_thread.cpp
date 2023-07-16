#include <pthread.h>
#include <time.h>

#include <atomic>

#include <ui/userint.h>
#include <log_client.h>

namespace saglobal {
    // Main UI player object, resides all other objects needed to do text/image graphics render in screen
    UiClientUser* g_playerUi;
    std::atomic<bool> g_clientHasInitiliazed{false};
    
    std::atomic<bool> g_clientIsRunning{false};
}

namespace saclient {

    using namespace saglobal;

    pthread_cond_t g_multCond;
    pthread_mutex_t g_multExclusive;
    
    void exitingFromGame()
    {
        pthread_cond_destroy(&g_multCond);
        pthread_mutex_destroy(&g_multExclusive);
    }

    void* enterMainLoop([[maybe_unused]] void* unused)
    {
        pthread_cond_init(&g_multCond, nullptr);
        pthread_mutex_init(&g_multExclusive, nullptr);
        
        g_clientHasInitiliazed = true;
        g_playerUi = new UiClientUser();
        
        // Waiting for player press the Multiplayer button
        pthread_mutex_lock(&g_multExclusive);
        pthread_cond_wait(&g_multCond, &g_multExclusive);
        
        salog::print(ANDROID_LOG_INFO, "Multiplayer game thread has continued");
        
        for (;;) {
            g_clientIsRunning = true;
            
            struct timespec requestWait{.tv_sec = 10};
            nanosleep(&requestWait, nullptr);
        }
        
        
        return nullptr;
    }

}
