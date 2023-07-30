#include <log_client.h>

#include <game/game_objects.h>

namespace saglobal {
    void (*g_Render2dStuff)(void);
}

namespace samimic {
    // Responsible for rendering both overlay buttons and the user player's graphical user interface (GUI)
    
    void Render2dStuff()
    {
        static uint64_t callTimes{};
        salog::printFormat(salog::Info, "Render2dStuff was been called by %ld times", callTimes++);

        saglobal::g_Render2dStuff();

        if (saglobal::g_playerUi)
            saglobal::g_playerUi->renderDrawCommand();
    }
}
