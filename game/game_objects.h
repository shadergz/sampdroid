
#include <cstdint>
#include <ui/user_graphics.h>

namespace samimic {
    uint64_t CGame_InitializeRenderWare();
    void CClock_Update(uint64_t rX0);
}

namespace saglobal {
    extern uintptr_t g_gameAddr;
    extern UiClientUser* g_playerUi;

    extern uint64_t (*g_CGame_InitializeRenderWare)();
    extern void (*g_CClock_Update)(uint64_t rX0);

}
