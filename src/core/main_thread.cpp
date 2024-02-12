#include <pthread.h>
#include <time.h>
#include <jni.h>

#include <atomic>

#include <core/log_client.h>
#include <ui/user_graphics.h>

// Main UI player object, resides all other objects needed to do text/image graphics render in screen
UiClientUser* g_playerUi;
std::atomic<bool> g_clientHasInitiliazed{false};
std::atomic<bool> g_clientIsRunning{false};
extern JNIEnv* g_gameEnv;

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
    useriDsp("Main: main thread has started!");

    static JavaVMAttachArgs threadInfo{.version = JNI_VERSION_1_6, .name = "Main Loop"};
    JavaVM* vm{};
    g_gameEnv->GetJavaVM(&vm);
    vm->AttachCurrentThread(&g_gameEnv, &threadInfo);

    pthread_mutex_lock(&g_multExclusive);
    pthread_cond_wait(&g_multCond, &g_multExclusive);

    // We're in onStart() JVM from GTASA class
    // Our JNI_OnLoad function has already completed its execution

    g_clientIsRunning = true;
    useriDsp("Main: multiplayer game thread has continued");

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
