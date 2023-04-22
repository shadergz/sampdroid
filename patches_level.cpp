
#include <patches_level.h>
#include <menu_handler.h>
#include <outside.h>

#include <unistd.h>
#include <sys/mman.h>

AArch64_Patcher* g_patcher_micro;
extern uintptr_t g_game_addr;

void AArch64_Patcher::replaceMethod(const uintptr_t method, 
    const uintptr_t replace, uintptr_t* save_in)
{
    mtmprintf(ANDROID_LOG_INFO, 
        "hooking function %#lx with %#lx method, saving in %#lx", method, replace, save_in);
    
}
void AArch64_Patcher::unfuckPageRWX(uintptr_t unfuck_addr, uint64_t region_size)
{
    const auto base_addr{unfuck_addr & 0xfffff000u};
    // If page isn't aligned we can't change the permission for more than once page 
    // without break into multiples
    const auto protect{PROT_READ|PROT_WRITE|PROT_EXEC};
    const auto page_size{getpagesize()};
    
    auto count_pages = [page_size](const auto size) -> auto {
        uintptr_t count{size / page_size};
        if (size % page_size)
            count++;
        return count;
    };
    
    auto count{count_pages(region_size)};
    mtmprintf(ANDROID_LOG_INFO, 
        "changing permission of %lu pages in %#lx base address", count, base_addr);
    mprotect((void*)(base_addr), count * page_size, protect);
}


void applyGlobalPatches() 
{
    g_patcher_micro = new AArch64_Patcher();
    g_patcher_micro->replaceMethod(g_game_addr+0x358010, (uintptr_t)(MainMenuScreen_AddAllItems_HOOK), (uintptr_t*)(&MainMenuScreen_AddAllItems));
    
}