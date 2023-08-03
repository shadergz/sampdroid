
#include <game/game_objects.h>
#include <log_client.h>

namespace saglobal {
}

namespace samimic {

    void CEntity_UpdateRwFrame(uintptr_t thiz)
    {
        salog::printFormat(salog::Info, "UpdateRwFrame() has called, entity struct ptr: %#p", thiz);

        if (saglobal::g_playerUi)
            saglobal::g_playerUi->renderByEachGameSecond();
    }
}