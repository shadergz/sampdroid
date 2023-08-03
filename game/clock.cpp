
#include <game/game_objects.h>
#include <log_client.h>

namespace saglobal {
    void (*g_CClock_Update)(uint64_t rX0);
}

namespace samimic {

    // Updates the game clock every second based on the current FPS, which could be either after 60 frames or 30 frames, depending on the situation
    void CClock_Update(uint64_t rX0)
    {
        static uint64_t callTimes{};
        salog::printFormat(salog::Debug, 
            "Game world clock has been incremented since the spawn by %ld", callTimes++);

        saglobal::g_CClock_Update(rX0);
    }
}
