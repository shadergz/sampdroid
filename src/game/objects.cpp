#include <pthread.h>

#include <game/objects.h>
extern void initSaReferences();
uint64_t (*g_CGame_InitialiseRenderWare)();

extern pthread_cond_t g_multCond;
extern pthread_mutex_t g_multExclusive;

uint64_t CGame_InitialiseRenderWare()
{
    auto rwResult = g_CGame_InitialiseRenderWare();
    // Loads RenderWare global variables and functions pointer
    initSaReferences();
    using TextureDatabaseFormat = int32_t;

    // This functions below doesn't clashes the game if the texture database wasn't found
    ((int64_t (*)(const char*, bool, TextureDatabaseFormat))
        (g_gameAddr + 0x28771c))("client", false, 5);
    ((int64_t (*)(const char*, bool, TextureDatabaseFormat))
        (g_gameAddr + 0x28771c))("playerside", false, 5);
    ((int64_t (*)(const char*, bool, TextureDatabaseFormat))
        (g_gameAddr + 0x28771c))("serverside", false, 5);

    pthread_mutex_lock(&g_multExclusive);
    pthread_cond_broadcast(&g_multCond);

    pthread_mutex_unlock(&g_multExclusive);

    return rwResult;
}
