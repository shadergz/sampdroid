
#include <cstdint>
#include <ui/user_graphics.h>

namespace samimic {
    uint64_t CGame_InitialiseRenderWare();
    void CClock_Update(uint64_t rX0);

    void CEntity_UpdateRpHAnim(void* __restrict thiz);
    
    void CHUD_Draw();
    
}

namespace saglobal {
    extern uintptr_t g_gameAddr;
    extern UiClientUser* g_playerUi;

    extern uint64_t (*g_CGame_InitialiseRenderWare)();
    extern void (*g_CClock_Update)(uint64_t rX0);

}
