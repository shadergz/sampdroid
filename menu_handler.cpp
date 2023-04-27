#include <cstdint>

#include <RenderWare/rwcore.h>
#include <menu_handler.h>
#include <texture_runtime.h>
#include <outside.h>

// Method signature: MainMenuScreen::AddAllItems(MainMenuScreen *this)
// Parameters: 
void (*MainMenuScreen_AddAllItems)(uintptr_t x0);
// Why CP? are you kidding with me?!
uint32_t (*MainMenuScreen_HasCPSave)();

#pragma pack(push, 1)


struct MenuSlot_ {
    RwTexture* m_button_texure;
    const char* m_fep_mask;
    void (*m_onPressedCallback)();
};

struct MainMenuScreen_ {
public:
    uint8_t m_undefined_0[0x15];
    bool    m_inGameplayScene;
    uint8_t m_undefined_1[0x42];

    /* A slot index identifier, needed to be increased while adding
     * new items into it! */

    // Count of available entries inside of m_slot
    uint32_t m_slot_max;
    uint32_t m_slot_index;

    MenuSlot_* m_slot;
};


#pragma pack(pop)

static_assert(offsetof(MainMenuScreen_, m_inGameplayScene) == 0x15);
static_assert(offsetof(MainMenuScreen_, m_slot_max) == 0x58);
static_assert(offsetof(MainMenuScreen_, m_slot_index) == 0x5c);
static_assert(offsetof(MainMenuScreen_, m_slot) == 0x60);

extern uintptr_t g_game_addr;

static constexpr const char BNAME_ON_RESUME[]{"menu_mainresume"};
static constexpr const char BNAME_ON_PLAY[]{"menu_mainplay"};
static constexpr const char BNAME_ON_SETTINGS[]{"menu_mainsettings"};

static constexpr const char BNAME_ON_BRIEFS[]{"menu_mainbriefs"};

static constexpr const char BNAME_ON_STATS[]{"menu_mainstats"};
static constexpr const char BNAME_ON_QUIT[]{"menu_mainquit"};

void (*OnResume_buttonPressed)();
void (*OnStartGame_buttonPressed)();
void (*OnSettings_buttonPressed)();
void (*OnBriefs_buttonPressed)();
void (*OnStats_buttonPressed)();
void (*OnExit_buttonPressed)();

static void menu_placeButton(
    const char* bt_name, const char fep[8], MainMenuScreen_* menu) 
{
    RwTexture* texture_bt{(RwTexture*)textureLoadNew("gta3", bt_name)};
    MTM_RUNTIME_ASSERT(texture_bt != nullptr, "Can't build the menu, some textures hasn't found!");
    
    auto slot_placeholder{menu->m_slot_index};
    mtmprintf(ANDROID_LOG_DEBUG, "Menu slot index: %u\n", slot_placeholder);
    const uint32_t newSlot{slot_placeholder + 0x1};

    static constexpr uint SLOT_MAX_COUNT{8};
    if (!menu->m_slot) {
        mtmprintf(ANDROID_LOG_DEBUG, "Menu slot doesn't exist, allocating 8 slots now!");
        // TODO: Maybe the game clashes because of this, the original function allocate with malloc instead of new operator
        menu->m_slot = new MenuSlot_[SLOT_MAX_COUNT];
        // Putting a trap data into it (this has used for debug purposes only!)!
        memset(menu->m_slot, 0xf, sizeof (MenuSlot_) * SLOT_MAX_COUNT);

        menu->m_slot_max = SLOT_MAX_COUNT;
    }
    MTM_RUNTIME_ASSERT(newSlot < menu->m_slot_max,
        "Can't use a slot for store the menu item with name: %s", bt_name);

    auto slot_ptr{&menu->m_slot[menu->m_slot_index++]};
    mtmprintf(ANDROID_LOG_INFO, "Free slot selected: %llx\n", slot_ptr);

    slot_ptr->m_button_texure = texture_bt;
    slot_ptr->m_fep_mask = fep;
    
    // Selecting the correct button callback
    if (!strncmp(bt_name, BNAME_ON_RESUME, sizeof BNAME_ON_RESUME))
        slot_ptr->m_onPressedCallback = OnResume_buttonPressed;
    else if (!strncmp(bt_name, BNAME_ON_PLAY, sizeof BNAME_ON_PLAY))
        slot_ptr->m_onPressedCallback = OnStartGame_buttonPressed;
    else if (!strncmp(bt_name, BNAME_ON_SETTINGS, sizeof BNAME_ON_SETTINGS))
        slot_ptr->m_onPressedCallback = OnSettings_buttonPressed;
    else if (!strncmp(bt_name, BNAME_ON_BRIEFS, sizeof BNAME_ON_SETTINGS))
        slot_ptr->m_onPressedCallback = OnBriefs_buttonPressed;
    else if (!strncmp(bt_name, BNAME_ON_STATS, sizeof BNAME_ON_SETTINGS))
        slot_ptr->m_onPressedCallback = OnStats_buttonPressed;
    else if (!strncmp(bt_name, BNAME_ON_QUIT, sizeof BNAME_ON_SETTINGS))
        slot_ptr->m_onPressedCallback = OnExit_buttonPressed;

}

void MainMenuScreen_AddAllItems_HOOK(uintptr_t this_x0)
{
    mtmputs(ANDROID_LOG_WARN, "MenuHook: on (AddAllItems)!");
    // mtmprintf(ANDROID_LOG_INFO, "Discarding the original function %#llx", MainMenuScreen_AddAllItems);

    *(uintptr_t*)&MainMenuScreen_HasCPSave = g_game_addr+0x35a680;
    /* It's seems that the original function is trying to detect if we are at
     * the main game screen or in gameplay game screen scene (Like when we open the map 
     * while in gameplay)
     * 1. [x0 + 0x15] has a value (boolean) that determine this!
     *    it's clearly a boolean value
     *    mov        x19, this
     *    ldrb       w8, [x19, #0x15] */

    MainMenuScreen_* our_inGameMenu{reinterpret_cast<MainMenuScreen_*>(this_x0)};
    mtmprintf(ANDROID_LOG_INFO, "MenuHook: menu structure location: %llx\n", our_inGameMenu);
    
    *reinterpret_cast<uintptr_t*>(&OnResume_buttonPressed)    = g_game_addr+0x35a0f8;
    *reinterpret_cast<uintptr_t*>(&OnStartGame_buttonPressed) = g_game_addr+0x35a31c;
    *reinterpret_cast<uintptr_t*>(&OnSettings_buttonPressed)  = g_game_addr+0x35a208;
    // void FlowScreen::OnStats(void)
    *reinterpret_cast<uintptr_t*>(&OnStats_buttonPressed)     = g_game_addr+0x35a244;
    // void FlowScreen::OnBriefs(void)
    *reinterpret_cast<uintptr_t*>(&OnBriefs_buttonPressed)    = g_game_addr+0x35a2b0;
    
    *reinterpret_cast<uintptr_t*>(&OnExit_buttonPressed)      = g_game_addr+0x35a758;

    mtmputs(ANDROID_LOG_INFO, "MenuHook: placing on game menu buttons");

    if (__builtin_expect(!our_inGameMenu->m_inGameplayScene, 0)) {
        // We're in the main menu, the user can choice between SAMP or MTA
        mtmputs(ANDROID_LOG_WARN, "MenuHook: placing Main Menu (Resume) button");
        // For place the "Resume button we need to check if there's a Save Game already"
        uint32_t hasSave{MainMenuScreen_HasCPSave()};
        if (hasSave & 1)
            menu_placeButton(BNAME_ON_RESUME, "FEP_RES", our_inGameMenu);
        
        menu_placeButton(BNAME_ON_PLAY, "FEP_STG", our_inGameMenu);
        menu_placeButton(BNAME_ON_SETTINGS, "FEP_OPT", our_inGameMenu);
    } else {
    
        menu_placeButton(BNAME_ON_RESUME, "FEP_RES", our_inGameMenu);
        menu_placeButton(BNAME_ON_SETTINGS, "FEP_OPT", our_inGameMenu);
        menu_placeButton(BNAME_ON_STATS, "FEH_STA", our_inGameMenu);
    
        // Checking if the player has new messages, whether yes...
        if (*reinterpret_cast<uint64_t*>(g_game_addr+0xca04d0))
            // Putting the "Briefs" button
            menu_placeButton(BNAME_ON_BRIEFS, "FEH_BRI", our_inGameMenu);

        menu_placeButton(BNAME_ON_PLAY, "FEP_STG", our_inGameMenu);
    }

    if (our_inGameMenu->m_slot_index + 1 <= our_inGameMenu->m_slot_max)
        // This action always needed to be successful completed
        menu_placeButton(BNAME_ON_QUIT, "FEP_QUI", our_inGameMenu);

}

