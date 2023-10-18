
#include <inj/patches_notes.h>
#include <client/log_client.h>

#include "objects.h"

namespace saglobal {
    void (*g_CClock_Update)(uint64_t rX0);
}

namespace samimic {

    // Updates the game clock every second based on the current FPS, which could be either after 60 frames or 30 frames, depending on the situation
    void CClock_Update(uint64_t rX0)
    {
        SAVE_REGS_07();

        static uint64_t secsIncr{};
        salog::printFormat(salog::Debug, "Game world clock has been incremented since the spawn by %ld", secsIncr++);

        RESTORE_REGS_70();
        saglobal::g_CClock_Update(rX0);
    }
}
