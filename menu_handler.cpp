#include <menu_handler.h>

#include <outside.h>

// method signature: MainMenuScreen::AddAllItems(MainMenuScreen *this)
// parameters: 

void (*MainMenuScreen_AddAllItems)(uintptr_t x0);
void MainMenuScreen_AddAllItems_HOOK(uintptr_t x0)
{
    mtmcout(ANDROID_LOG_INFO, "On (AddAllItems)");
    // we're running before MainMenuScreen::AddAllItems
    MainMenuScreen_AddAllItems(x0);
}

