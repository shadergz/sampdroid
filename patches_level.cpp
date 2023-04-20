#include <patches_level.h>
#include <menu_handler.h>
#include <outside.h>

#include <unistd.h>
#include <sys/mman.h>

AArch64_Patcher* patcher_micro;
extern uintptr_t g_gameAddr;

void AArch64_Patcher::rtReplaceMethod(
    const uintptr_t method, 
    const uintptr_t replace, 
    uintptr_t* saveIn)
{
    mtmprintf(ANDROID_LOG_INFO, "Hooking function %#lx with %#lx method, saving in %#lx",
        method, replace, saveIn);
    
}
void AArch64_Patcher::unfuckPageRWX(uintptr_t unfuckAddr, uint64_t regionSize)
{
    const auto baseAddr{unfuckAddr & 0x0000};
    // We can't change the permission for more than once thread
    const auto protect{PROT_READ|PROT_WRITE|PROT_EXEC};
    const auto pageSize{getpagesize()};
    uint64_t pageCount = 1;
    while ((regionSize % pageSize) != 0) {
        pageCount++;
        regionSize /= pageSize;
    }

    mprotect(reinterpret_cast<void*>(baseAddr), 
        pageCount * pageSize, protect);
}


void applyGlobalPatches() 
{
    patcher_micro = new AArch64_Patcher();
    patcher_micro->rtReplaceMethod(g_gameAddr+0x00358010,
        reinterpret_cast<uintptr_t>(MainMenuScreen_AddAllItems_HOOK),
        reinterpret_cast<uintptr_t*>(&MainMenuScreen_AddAllItems));
    
}