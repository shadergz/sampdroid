
#include <patches_level.h>
#include <menu_handler.h>
#include <outside.h>

#include <unistd.h>
#include <sys/mman.h>

AArch64_Patcher* patcher_micro;
extern uintptr_t g_gameAddr;

void AArch64_Patcher::rtReplaceMethod(const uintptr_t method, const uintptr_t replace, 
    uintptr_t* saveIn)
{
    mtmprintf(ANDROID_LOG_INFO, 
        "Hooking function %#lx with %#lx method, saving in %#lx", method, replace, saveIn);
    
}
void AArch64_Patcher::unfuckPageRWX(uintptr_t unfuckAddr, uint64_t regionSize)
{
    const auto baseAddr{unfuckAddr & 0xfffff000u};
    // If page isn't aligned we can't change the permission for more than once page 
    // without break into multiples
    const auto protect{PROT_READ|PROT_WRITE|PROT_EXEC};
    const auto pageSize{getpagesize()};
    
    auto countPages = [pageSize](const auto size) -> auto {
        uintptr_t count{size/pageSize};
        if (size%pageSize)
            count++;
        return count;
    };
    
    auto count{countPages(regionSize)};
    mtmprintf(ANDROID_LOG_INFO, 
        "Changing permission of %lu pages in %#lx base address", count, baseAddr);
    mprotect((void*)(baseAddr), count*pageSize, protect);
}


void applyGlobalPatches() 
{
    patcher_micro = new AArch64_Patcher();
    patcher_micro->rtReplaceMethod(g_gameAddr+0x358010, (uintptr_t)(MainMenuScreen_AddAllItems_HOOK), (uintptr_t*)(&MainMenuScreen_AddAllItems));
    
}