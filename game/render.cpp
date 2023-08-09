#include <malloc.h>

#include <patches_notes.h>
#include <game/game_objects.h>
#include <log_client.h>

namespace saglobal {
    void (*g_CTouchInterface_DrawAll)(bool drawAll);
}

namespace samimic {

    void CEntity_UpdateRpHAnim(void* __restrict thiz)
    {
        SAVE_REGS_07();

        salog::printFormat(salog::Debug, 
            "Player CJ body animation updated with CEntity struct packet ptr: %#lx [allocated size: %lfKib]", 
            thiz, malloc_usable_size(thiz) / 1024);

        RESTORE_REGS_70();
    }

    void CTouchInterface_DrawAll(bool drawAll)
    {
        saglobal::g_CTouchInterface_DrawAll(drawAll);

        salog::printFormat(
            salog::Debug, "CTouchInterface_DrawAll called with parameter boolean: %s", 
            drawAll ? "true" : "false");

        if (saglobal::g_playerUi)
            saglobal::g_playerUi->renderByEachGameSecond();

    }
}