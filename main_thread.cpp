#include <pthread.h>
#include <time.h>

#include <atomic>

#include <ui/user_graphics.h>
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

        g_clientIsRunning = false;
    }

    void* enterMainLoop([[maybe_unused]] void* unused)
    {
        g_clientHasInitiliazed = true;        
        salog::print(salog::LogId::Info, "Main thread has started!");

        pthread_mutex_lock(&g_multExclusive);
        pthread_cond_wait(&g_multCond, &g_multExclusive);

        // We're in onStart() JVM from GTASA class
        // Our JNI_OnLoad function has already completed its execution

        g_clientIsRunning = true;
        salog::print(salog::LogId::Debug, "Multiplayer game thread has continued");

        // Waiting for the player press the Start button
        pthread_cond_wait(&g_multCond, &g_multExclusive);

        g_playerUi = new UiClientUser();

        for (;;) {
            // Main Multiplayer indepedent thread loop      
            const struct timespec requestWait{.tv_sec = 10};
            nanosleep(&requestWait, nullptr);
        }

        exitingFromGame();

        pthread_exit(nullptr);
        return nullptr;
    }

}
