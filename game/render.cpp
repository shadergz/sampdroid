#include <malloc.h>

#include <game/game_objects.h>
#include <log_client.h>

namespace samimic {

    void CEntity_UpdateRpHAnim(void* __restrict thiz)
    {
        salog::printFormat(salog::Info, 
            "UpdateRpHAnim() has called, CEntity struct ptr: %#lx [allocated size: %lfKib]", 
            thiz, static_cast<float>(malloc_usable_size(thiz)) / 1024);

    }

    void CHUD_Draw()
    {
        // __asm("brk #0");
        salog::print(salog::Info, "CHUD_Draw called");

        // if (saglobal::g_playerUi)
        //    saglobal::g_playerUi->renderByEachGameSecond();
    }
}