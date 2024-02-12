#include <malloc.h>
#include <inj/patches_notes.h>
#include <user/log_client.h>
#include <game/objects.h>

void (*g_CTouchInterface_DrawAll)(bool drawAll);
void CEntity_UpdateRpHAnim(void* __restrict thiz)
{
    SAVE_REGS_07();
    userdDsp("Player CJ body animation updated with CEntity struct packet ptr: %#lx [allocated size: %lfKib]",
        thiz, malloc_usable_size(thiz) / 1024);
    RESTORE_REGS_70();
}

void CTouchInterface_DrawAll(bool drawAll)
{
    g_CTouchInterface_DrawAll(drawAll);
    userdDsp("CTouchInterface_DrawAll called with parameter boolean: %s", drawAll ? "true" : "false");
    if (g_playerUi)
        g_playerUi->renderOnGameScene();
}