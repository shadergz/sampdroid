
#include <game/game_objects.h>
#include <log_client.h>

namespace saglobal {
}

namespace samimic {

    void CEntity_UpdateRpHAnim(uintptr_t thiz)
    {
        // __asm("brk #0\n");

        salog::printFormat(salog::Info, "UpdateRpHAnim() has called, CEntity struct ptr: %#p", thiz);

        // if (saglobal::g_playerUi)
        //     saglobal::g_playerUi->renderByEachGameSecond();
    }
}