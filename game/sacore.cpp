
#include <game/sacore.h>
#include <log_client.h>

namespace sarender {
    extern void initSaReferences();
};

namespace saglobal {
    extern uintptr_t g_gameAddr;
}

namespace samimic {
    CGame::CGame() {}
    CGame::~CGame() {}

    // Inside CGame::InitialiseRenderWare() the game will actually loads all textures database of the game
    // that includes: ["mobile", "txd", "gta3", "gta_int", "cutscene", "menu"]
    // The menu database has all textures from the game menu screen and inside the gameplay
    // We know what the string parameter is, but we can't figure out at this moment, what the false, and 6 are...
    
    // https://gtamods.com/wiki/Mobile_textures_(SA/VC)
    
    const char* CGame::InitialiseRenderWare()
    {
        salog::print(ANDROID_LOG_WARN, "Loading the RenderWare core engine and our databases");
        sarender::initSaReferences();

        using TextureDatabaseFormat = int;

        ((int64_t (*)(const char*, bool, TextureDatabaseFormat))(saglobal::g_gameAddr + 0x28771c))("samobile", false, 5);
        ((int64_t (*)(const char*, bool, TextureDatabaseFormat))(saglobal::g_gameAddr + 0x28771c))("playerside", false, 5);
        ((int64_t (*)(const char*, bool, TextureDatabaseFormat))(saglobal::g_gameAddr + 0x28771c))("serverside", false, 5);

        return nullptr;
    }
}