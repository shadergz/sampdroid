
#include <cstdint>
#include <ui/user_graphics.h>

namespace samimic {
    long CGame_InitializeRenderWare();
    void Render2dStuff(void);
}

namespace saglobal {
    extern uintptr_t g_gameAddr;
    extern UiClientUser* g_playerUi;

    extern long (*g_CGame_InitializeRenderWare)();
    extern void (*g_Render2dStuff)(void);
}
