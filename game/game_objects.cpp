#include <pthread.h>

#include <game/game_objects.h>

namespace sarender {
    extern void initSaReferences();
};

namespace saglobal {
    uint64_t (*g_CGame_InitialiseRenderWare)();
}

namespace appclient {
    extern pthread_cond_t g_multCond;
    extern pthread_mutex_t g_multExclusive;
}

namespace samimic {
    using namespace saglobal;
    uint64_t CGame_InitialiseRenderWare()
    {
        auto rwResult{g_CGame_InitialiseRenderWare()};
        
        // Loads RenderWare global variables and functions pointer
        sarender::initSaReferences();

        using TextureDatabaseFormat = int32_t;

        // This functions below doesn't clashes the game if the texture database wasn't found
        ((int64_t (*)(const char*, bool, TextureDatabaseFormat))
            (saglobal::g_gameAddr + 0x28771c))("client", false, 5);
        
        ((int64_t (*)(const char*, bool, TextureDatabaseFormat))
            (saglobal::g_gameAddr + 0x28771c))("playerside", false, 5);
        
        ((int64_t (*)(const char*, bool, TextureDatabaseFormat))
            (saglobal::g_gameAddr + 0x28771c))("serverside", false, 5);

        pthread_mutex_lock(&appclient::g_multExclusive);
        pthread_cond_broadcast(&appclient::g_multCond);
        
        pthread_mutex_unlock(&appclient::g_multExclusive);

        return rwResult;
    }
}
