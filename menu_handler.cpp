#include <cstdint>

#include <rwrefs/rwcore.h>
#include <menu_handler.h>
#include <texture_runtime.h>
#include <outside.h>

namespace Mt4Global {
bool g_playMultiplayer{false};
void (*g_MainMenuScreen_AddAllItems)(uintptr_t x0);


extern uintptr_t g_gameAddr;
}

// Why CP? are you kidding with me?!
static uint32_t (*MainMenuScreen_HasCPSave)();

#pragma pack(push, 1)


struct MenuSlot_ {
    RwTexture* m_buttonTexure;
    const char* m_fepMask;
    void (*m_onPressedCallback)();
};

struct MainMenuScreen_ {
public:
    uint8_t m_undefined0[0x15];
    bool    m_inGameplayScene;
    uint8_t m_undefined1[0x42];

    /* A slot index identifier, needed to be increased while adding
     * new items into it! */

    // Count of available entries inside of m_slot
    uint32_t m_slotMax;
    uint32_t m_slotIndex;

    MenuSlot_* m_slot;
};


#pragma pack(pop)

static_assert(offsetof(MainMenuScreen_, m_inGameplayScene) == 0x15);
static_assert(offsetof(MainMenuScreen_, m_slotMax) == 0x58);
static_assert(offsetof(MainMenuScreen_, m_slotIndex) == 0x5c);
static_assert(offsetof(MainMenuScreen_, m_slot) == 0x60);

static constexpr const char BNAME_ON_RESUME[]{"menu_mainresume"};
static constexpr const char BNAME_ON_PLAY[]{"menu_mainplay"};
static constexpr const char BNAME_ON_SETTINGS[]{"menu_mainsettings"};

static constexpr const char BNAME_ON_BRIEFS[]{"menu_mainbriefs"};

static constexpr const char BNAME_ON_STATS[]{"menu_mainstats"};
static constexpr const char BNAME_ON_QUIT[]{"menu_mainquit"};

static void (*OnResume_buttonPressed)();
static void (*OnStartGame_buttonPressed)();
static void (*OnSettings_buttonPressed)();
static void (*OnBriefs_buttonPressed)();
static void (*OnStats_buttonPressed)();
static void (*OnExit_buttonPressed)();



namespace Mt4Context {

void menuOnInit() {

}

}

namespace Mt4Handler {
[[maybe_unused]] static void OnMultiplayer_buttonPressed() 
{
    if (Mt4Global::g_playMultiplayer)
        return;

    const time_t t{time(NULL)};
    struct tm *tm{localtime(&t)};
    char tMsg[0x1f];
    strftime(tMsg, sizeof tMsg, "%c", tm);

    Mt4Global::g_playMultiplayer = true;
    Mt4Context::menuOnInit();

    Mt4Log::printflike(ANDROID_LOG_INFO, "multiplayer button selected from menu at %s, enjoy!", tMsg);
}
}

namespace Mt4Mimic {
using namespace Mt4Global;

static void menu_placeButton(
    const char* btName, const char fep[8], MainMenuScreen_* menu) 
{
    auto textureButton{(RwTexture*)g_textureDatabase->textureLoadNew("gta3", btName)};
    MT4LOG_RUNTIME_ASSERT(textureButton != nullptr, "Can't build the menu, some textures hasn't found!");
    
    auto slotPlaceholder{menu->m_slotIndex};
    Mt4Log::printflike(ANDROID_LOG_DEBUG, "Menu slot index: %u\n", slotPlaceholder);
    const uint32_t newSlot{slotPlaceholder + 0x1};

    static constexpr uint SLOT_MAX_COUNT{8};
    if (!menu->m_slot) {
        Mt4Log::printflike(ANDROID_LOG_DEBUG, "Menu slot doesn't exist, allocating 8 slots now!");
        // TODO: Maybe the game clashes because of this, the original function allocate with malloc instead of new operator
        menu->m_slot = new MenuSlot_[SLOT_MAX_COUNT];
        // Putting a trap data into it (this has used for debug purposes only!)!
        memset(menu->m_slot, 0xf, sizeof (MenuSlot_) * SLOT_MAX_COUNT);

        menu->m_slotMax = SLOT_MAX_COUNT;
    }
    MT4LOG_RUNTIME_ASSERT(newSlot < menu->m_slotMax,
        "Can't use a slot for store the menu item with name: %s", bt_name);

    auto slotPtr{&menu->m_slot[menu->m_slotIndex++]};
    Mt4Log::printflike(ANDROID_LOG_INFO, "Free slot selected: %llx\n", slotPtr);

    slotPtr->m_buttonTexure = textureButton;
    slotPtr->m_fepMask = fep;
    
    // Selecting the correct button callback
    if (!strncmp(btName, BNAME_ON_RESUME, sizeof BNAME_ON_RESUME))
        slotPtr->m_onPressedCallback = OnResume_buttonPressed;
    else if (!strncmp(btName, BNAME_ON_PLAY, sizeof BNAME_ON_PLAY))
        slotPtr->m_onPressedCallback = OnStartGame_buttonPressed;
    else if (!strncmp(btName, BNAME_ON_SETTINGS, sizeof BNAME_ON_SETTINGS))
        slotPtr->m_onPressedCallback = OnSettings_buttonPressed;
    else if (!strncmp(btName, BNAME_ON_BRIEFS, sizeof BNAME_ON_SETTINGS))
        slotPtr->m_onPressedCallback = OnBriefs_buttonPressed;
    else if (!strncmp(btName, BNAME_ON_STATS, sizeof BNAME_ON_SETTINGS))
        slotPtr->m_onPressedCallback = OnStats_buttonPressed;
    else if (!strncmp(btName, BNAME_ON_QUIT, sizeof BNAME_ON_SETTINGS))
        slotPtr->m_onPressedCallback = OnExit_buttonPressed;

}

void MainMenuScreen_AddAllItems(uintptr_t this_x0)
{
    Mt4Log::putslike(ANDROID_LOG_WARN, "MenuHook: on (AddAllItems)!");
    // Mt4Log::printflike(ANDROID_LOG_INFO, "Discarding the original function %#llx", MainMenuScreen_AddAllItems);

    *(uintptr_t*)&MainMenuScreen_HasCPSave = g_gameAddr+0x35a680;
    /* It's seems that the original function is trying to detect if we are at
     * the main game screen or in gameplay game screen scene (Like when we open the map 
     * while in gameplay)
     * 1. [x0 + 0x15] has a value (boolean) that determine this!
     *    it's clearly a boolean value
     *    mov        x19, this
     *    ldrb       w8, [x19, #0x15] */

    MainMenuScreen_* ourInGameMenu{reinterpret_cast<MainMenuScreen_*>(this_x0)};
    Mt4Log::printflike(ANDROID_LOG_INFO, "MenuHook: menu structure location: %llx\n", ourInGameMenu);
    
    *reinterpret_cast<uintptr_t*>(&OnResume_buttonPressed)    = g_gameAddr+0x35a0f8;
    *reinterpret_cast<uintptr_t*>(&OnStartGame_buttonPressed) = g_gameAddr+0x35a31c;
    *reinterpret_cast<uintptr_t*>(&OnSettings_buttonPressed)  = g_gameAddr+0x35a208;
    // void FlowScreen::OnStats(void)
    *reinterpret_cast<uintptr_t*>(&OnStats_buttonPressed)     = g_gameAddr+0x35a244;
    // void FlowScreen::OnBriefs(void)
    *reinterpret_cast<uintptr_t*>(&OnBriefs_buttonPressed)    = g_gameAddr+0x35a2b0;
    
    *reinterpret_cast<uintptr_t*>(&OnExit_buttonPressed)      = g_gameAddr+0x35a758;

    Mt4Log::putslike(ANDROID_LOG_INFO, "MenuHook: placing on game menu buttons");

    if (__builtin_expect(!ourInGameMenu->m_inGameplayScene, 0)) {
        // We're in the Main Menu, the user can choice between SAMP or MTA
        Mt4Log::putslike(ANDROID_LOG_WARN, "MenuHook: placing Main Menu (Resume) button");
        // For place the "Resume button we need to check if there's a Save Game already"
        uint32_t hasSave{MainMenuScreen_HasCPSave()};
        if (hasSave & 1)
            menu_placeButton(BNAME_ON_RESUME, "FEP_RES", ourInGameMenu);
        
        menu_placeButton(BNAME_ON_PLAY, "FEP_STG", ourInGameMenu);
        menu_placeButton(BNAME_ON_SETTINGS, "FEP_OPT", ourInGameMenu);
    } else {
    
        menu_placeButton(BNAME_ON_RESUME, "FEP_RES", ourInGameMenu);
        menu_placeButton(BNAME_ON_SETTINGS, "FEP_OPT", ourInGameMenu);
        menu_placeButton(BNAME_ON_STATS, "FEH_STA", ourInGameMenu);
    
        // Checking if the player has new messages, whether yes...
        if (*reinterpret_cast<uint64_t*>(g_gameAddr+0xca04d0))
            // Putting the "Briefs" button
            menu_placeButton(BNAME_ON_BRIEFS, "FEH_BRI", ourInGameMenu);

        menu_placeButton(BNAME_ON_PLAY, "FEP_STG", ourInGameMenu);
    }

    if (ourInGameMenu->m_slotIndex + 1 <= ourInGameMenu->m_slotMax)
        // This action always needed to be successful completed
        menu_placeButton(BNAME_ON_QUIT, "FEP_QUI", ourInGameMenu);

}

}
