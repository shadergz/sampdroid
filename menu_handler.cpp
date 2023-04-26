#include <menu_handler.h>

#include <stdint.h>
#include <texture_runtime.h>

#include <outside.h>

// method signature: MainMenuScreen::AddAllItems(MainMenuScreen *this)
// parameters: 
void (*MainMenuScreen_AddAllItems)(uintptr_t x0);
// why CP? Are you kidding with me?!
uint32_t (*MainMenuScreen_HasCPSave)();

#pragma pack(push, 1)
struct MainMenuScreen_ {
public:
    uint8_t m_undefined_0[0x15];
    bool    m_inGameplayScene;
    uint8_t m_undefined_1[0x42];

    // a slot index identifier, needed to be increased while adding
    // new items into it!

    // count of available entries inside of m_slot
    uint32_t m_slot_sz;
    uint32_t m_slot_index;
    uintptr_t* m_slot;
};
#pragma pack(pop)

static_assert(offsetof(MainMenuScreen_, m_inGameplayScene) == 0x15);
static_assert(offsetof(MainMenuScreen_, m_slot_sz) == 0x58);
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


static void menuLoadButton(
    const char* bt_name,
    const char fep[8],
    MainMenuScreen_* menu) 
{
    uintptr_t texture_bt{textureLoadNew("?", bt_name)};
    auto texture_placeholder{menu->m_slot_index};
    uint32_t newSlot{texture_placeholder + 0x1};

    if (menu->m_slot_sz < newSlot) {}

    static constexpr uint SALT_SLOT_STSZ{0x18};

    uint64_t* slot_ptr{(uint64_t*)menu->m_slot[newSlot * SALT_SLOT_STSZ]};
    slot_ptr[0] = texture_bt;
    slot_ptr[1] = (uint64_t)*fep;
    
    if (!strncmp(bt_name, BNAME_ON_RESUME, sizeof BNAME_ON_RESUME))
        slot_ptr[2] = (uint64_t)OnResume_buttonPressed;
    else if (!strncmp(bt_name, BNAME_ON_PLAY, sizeof BNAME_ON_PLAY))
        slot_ptr[2] = (uint64_t)OnStartGame_buttonPressed;
    else if (!strncmp(bt_name, BNAME_ON_SETTINGS, sizeof BNAME_ON_SETTINGS))
        slot_ptr[2] = (uint64_t)OnSettings_buttonPressed;

    menu->m_slot_index++;
}

void MainMenuScreen_AddAllItems_HOOK(uintptr_t x0)
{
    mtmputs(ANDROID_LOG_WARN, "MenuHook: on (AddAllItems)!");
    mtmprintf(ANDROID_LOG_INFO, "discarding the original function %#llx", 
        MainMenuScreen_AddAllItems);

    *(uintptr_t*)&MainMenuScreen_HasCPSave = g_game_addr+0x35a680;
    // it's seems that the original function is trying to detect if we are at
    // the main game screen or in gameplay game screen scene (Like when we open the map 
    // while in gameplay)
    // 1. [x0 + 0x15] has a value (boolean) that determine this!
    //    it's clearly a boolean value
    //    mov        x19,this
    //    ldrb       w8,[x19, #0x15]

    MainMenuScreen_* our_inGameMenu{reinterpret_cast<MainMenuScreen_*>(x0)};
    *(uintptr_t*)&OnResume_buttonPressed = g_game_addr+0;

    mtmputs(ANDROID_LOG_INFO, "MenuHook: Placing on game menu buttons");

    if (!our_inGameMenu->m_inGameplayScene) {
        // we're in the main menu, the user can choice between SAMP or MTA
        mtmputs(ANDROID_LOG_WARN, "MenuHook: Placing the Resume button");
        // for place the "Resume button we need to check if there's a Save Game already"
        uint32_t hasSave{MainMenuScreen_HasCPSave()};
        if (hasSave & 1) {
            menuLoadButton(BNAME_ON_RESUME, "FEP_RES", our_inGameMenu);
        }
        menuLoadButton(BNAME_ON_PLAY, "FEP_STG", our_inGameMenu);
        menuLoadButton(BNAME_ON_SETTINGS, "FEP_OPT", our_inGameMenu);
        return;
    }
        
    menuLoadButton(BNAME_ON_RESUME, "FEP_RES", our_inGameMenu);
    menuLoadButton(BNAME_ON_SETTINGS, "FEP_OPT", our_inGameMenu);
    menuLoadButton(BNAME_ON_STATS, "FEH_STA", our_inGameMenu);
    
    if (*(uint32_t*)(g_game_addr+0x84a728))
        menuLoadButton(BNAME_ON_BRIEFS, "FEH_BRI", our_inGameMenu);

    menuLoadButton(BNAME_ON_PLAY, "FEP_STG", our_inGameMenu);
    menuLoadButton(BNAME_ON_QUIT, "FEP_QUI", our_inGameMenu);

}

