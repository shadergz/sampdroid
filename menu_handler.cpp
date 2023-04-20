#include <menu_handler.h>

// Origin: MainMenuScreen::AddAllItems(MainMenuScreen *this)
// Parameters: None (MainMenuScreen* this)

void (*MainMenuScreen_AddAllItems)(uintptr_t x0);
void MainMenuScreen_AddAllItems_HOOK(uintptr_t x0)
{
    // We're running before MainMenuScreen::AddAllItems
    MainMenuScreen_AddAllItems(x0);
}

