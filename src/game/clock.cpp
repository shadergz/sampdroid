#include <inj/patches_notes.h>
#include <client/log_client.h>

#include <game/objects.h>
void (*g_CClock_Update)(uint64_t rX0);
// Updates the game clock every second based on the current FPS,
// which could be either after 60 frames or 30 frames, depending on the situation
void CClock_Update(uint64_t rX0)
{
    SAVE_REGS_07();

    static uint64_t secsIncr{};
    userdDsp("Game world clock has been incremented since the spawn by %ld", secsIncr++);

    RESTORE_REGS_70();
    g_CClock_Update(rX0);
}
