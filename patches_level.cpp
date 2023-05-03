
#include "texture_runtime.h"
#include <cstdint>

#include <unistd.h>
#include <sys/user.h>
#include <sys/mman.h>

#include <patches_level.h>
#include <outside.h>

#include <menu_handler.h>
#include <nv_threads.h>

namespace Mt4Global {
AArch64_Patcher* g_patcherMicro;

extern uintptr_t g_gameAddr;
}

enum Micro_BranchMode {};

#pragma pack(push, 1)

struct Tr_Context {
    uint16_t m_id;
    uintptr_t m_source;
    // Count of instructions replaced inside the origin and placed inside of our trampoline
    uint8_t m_instCount;
    uint8_t m_mode;

    char m_symbName[AArch64_Patcher::PATCHER_SYMBOL_NAME];
    uint8_t m_trData[sizeof(uint32_t) * AArch64_Patcher::PATCHER_MAX_INST];
};

#pragma pack(pop)

static constexpr uint8_t PATCHER_FRAME_GOBACK = offsetof(Tr_Context, m_trData);

static_assert(sizeof(Tr_Context) == AArch64_Patcher::PATCHER_HOOK_SIZE,
    "Trampoline struct data size is invalid and must be fixed!");

void AArch64_Patcher::placeHookAt(const char* sbName, const uintptr_t method, 
    const uintptr_t replace, uintptr_t* saveIn)
{
    auto sbSize{strlen(sbName)};
    
    if (sbSize > sizeof Tr_Context::m_symbName) {
        Mt4Log::printflike(ANDROID_LOG_ERROR, "Symbol name %s is langer than the symbol name space!", sbName);
        *saveIn = 0; 
        return;
    }

    Mt4Log::printflike(ANDROID_LOG_INFO, 
        "Hooking function (%s) %#llx with %#llx method, "
            "saving in %#llx", sbName, method, replace, saveIn);
    auto trData{reinterpret_cast<Tr_Context*>(getNewTrampoline())};
    Mt4Log::printflike(ANDROID_LOG_INFO, "New trampoline allocated in %p\n", trData);
    
    if (!trData) { *saveIn = 0; return; }
    
    static uint16_t trampId{};
    
    trData->m_id = trampId++;
    trData->m_source = method;
    // 0 means an invalid value!

    strncpy(trData->m_symbName, sbName, 
        sizeof (trData->m_symbName));
    MT4LOG_RUNTIME_ASSERT((uintptr_t)trData->m_trData == (uintptr_t)trData + PATCHER_FRAME_GOBACK, 
        "PATCHER_FRAME_GOBACK isn't indexing the trampoline data as expected, please fix now!");
    
    // doing the frame backup (we're divorciating now)
    auto originFunc{(uint32_t*)method};
    *(uint32_t*)(trData->m_trData+0) = originFunc[0];
    *(uint32_t*)(trData->m_trData+4) = originFunc[1];
    *(uint32_t*)(trData->m_trData+8) = originFunc[2];
    *(uint32_t*)(trData->m_trData+12) = originFunc[3];

    trData->m_instCount = &originFunc[4] - &originFunc[0];

    // Installing our payload instructions

    /* Now we can overwrite the original method frame
     * making the page readable/writable and executable */
    unfuckPageRWX((uintptr_t)originFunc, PAGE_SIZE);
    // ldr x17, #0x8 -> loading a 64 bit immediate value from offset PC + 0x8
    *(originFunc+0) = __builtin_bswap32(0x51000058);
    // br x17
    *(originFunc+1) = __builtin_bswap32(0x20021fd6); 

    *(uint64_t**)(originFunc+2) = (uint64_t*)(replace & (uint64_t)-1);
    /* At this point, the trampoline has the 4 instructions from the origin instruction
     * and the original instruction has a jump to your hook method */
    *(uint64_t*)(trData->m_trData+16) = (uint64_t)__builtin_bswap64(0x5100005820021fd6);
    *(uint64_t**)(trData->m_trData+24) = (uint64_t*)((uint64_t)(originFunc+0x4) & (uint64_t)-1);    
    // Forcing the CPU to fetch the actual version for both operations 
    // Dumping the residual wrong instructions from the cache
    __builtin___clear_cache((char*)originFunc, (char*)&originFunc[4]);
    __builtin___clear_cache((char*)trData->m_trData, 
        (char*)&trData->m_trData[sizeof(trData->m_trData)]);

    Mt4Log::printflike(ANDROID_LOG_INFO, 
        "Hook on addr %#llx successful installed by %#llx, (| %#llx | %u |)",
        method, replace, (uintptr_t)trData->m_trData & 0xffffffffff, 
        trData->m_instCount);

    *saveIn = (uintptr_t)(trData->m_trData);
}
void AArch64_Patcher::unfuckPageRWX(uintptr_t unfuckAddr, uint64_t region_size)
{
    const auto baseAddr{unfuckAddr & 0xfffffff000u};
    /* If page isn't aligned we can't change the permission for more than once page 
     * without break into multiples */

    const auto protect{PROT_READ|PROT_WRITE|PROT_EXEC};
    const auto pageSize{getpagesize()};
    
    auto countPages = [pageSize](const auto size) -> auto {
        uintptr_t count{size / pageSize};
        if (size % pageSize)
            count++;
        return count;
    };

    auto overflow = unfuckAddr & 0xffff ? 1 : 0;
    auto count{countPages(region_size) + overflow};

    Mt4Log::printflike(ANDROID_LOG_INFO, 
        "Changing permission of %lu pages in %#llx base address", count, baseAddr);
    mprotect((void*)(baseAddr), count * pageSize, protect);
}

// While the game is loading we need to fixes and hook some functions
namespace Mt4Patches {

using namespace Mt4Global;

void applyOnGame()
{
    g_patcherMicro = new AArch64_Patcher();
    g_textureDatabase = new TextureDatabaseRuntime();

    // MenuItem_add is no longer present
    g_patcherMicro->placeHookAt("AddAllItems", 
        g_gameAddr+0x358010, 
        (uintptr_t)Mt4Mimic::MainMenuScreen_AddAllItems, 
        (uintptr_t*)&Mt4Global::g_MainMenuScreen_AddAllItems);
    
    g_patcherMicro->placeHookAt("NVThreadSpawnProc", 
        g_gameAddr+0x332040, 
        (uintptr_t)Mt4Mimic::NVThreadSpawnProc, 
        (uintptr_t*)&Mt4Global::g_NVThreadSpawnProc);


}

}
