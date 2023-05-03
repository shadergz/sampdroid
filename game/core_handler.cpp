
#include <game/game_raii.h>
#include <outside.h>

namespace Mt4RW {
    extern void initSAReferences();
};

namespace Mt4Global {
    extern uintptr_t g_gameAddr;
}

namespace Mt4Mimic 
{
CGame::CGame() {}
CGame::~CGame() {}

/* Inside CGame::InitialiseRenderWare() the game will actually loads all textures database of the game 
 * that includes: ["mobile", "txd", "gta3", "gta_int", "cutscene", "menu"]
 * The menu database has all textures from the game menu screen and inside the gameplay
*/
// https://gtamods.com/wiki/Mobile_textures_(SA/VC)
// We know what the string parameter is, but we can't figure out at this moment, what the false, and 6 are...

char* CGame::InitialiseRenderWare()
{
    Mt4Log::putslike(ANDROID_LOG_WARN, 
        "Initializing the RenderWare system and databases");
    Mt4RW::initSAReferences();

    using TextureDatabaseFormat = int;

    ((int64_t (*)(const char*, bool, TextureDatabaseFormat))(Mt4Global::g_gameAddr+0x28771c))("mt4m", false, 5);
    ((int64_t (*)(const char*, bool, TextureDatabaseFormat))(Mt4Global::g_gameAddr+0x28771c))("playerside", false, 5);
    ((int64_t (*)(const char*, bool, TextureDatabaseFormat))(Mt4Global::g_gameAddr+0x28771c))("serverside", false, 5);


    return nullptr;
}
}