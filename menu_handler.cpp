#include <menu_handler.h>

// method signature: MainMenuScreen::AddAllItems(MainMenuScreen *this)
// parameters: 

void (*MainMenuScreen_AddAllItems)(uintptr_t x0);
void MainMenuScreen_AddAllItems_HOOK(uintptr_t x0)
{
    // we're running before MainMenuScreen::AddAllItems
    MainMenuScreen_AddAllItems(x0);
}

