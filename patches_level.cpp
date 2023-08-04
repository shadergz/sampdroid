
#include <cstring>
#include <array>

#include <patches_level.h>
#include <patches_helper.h>

#include <texture_runtime.h>

#include <menu_handler.h>
#include <game/game_objects.h>

#include <nv_threads.h>

namespace saglobal {
    AArch64Patcher* g_patcherMicro;
    extern uintptr_t g_gameAddr;
}

#pragma pack(push, 4)

struct TrampolineContext {
    uint32_t m_id;
    uintptr_t m_source;
    
    // Count of instructions replaced inside the origin and placed inside of our trampoline
    uint8_t m_iCount;
    
    std::array<uint32_t, AArch64Patcher::PATCHER_MAX_INST> m_content;
};

#pragma pack(pop)

static_assert(sizeof(TrampolineContext) == AArch64Patcher::PATCHER_HOOK_SIZE,
    "Trampoline struct data size is invalid and should be fixed!");

#ifndef NDEBUG
#define DUMP_PRINT_INST(origin, limit)\
    salog::printFormat(salog::Info, "Hook: dump of %llu from method %#p", limit, origin);\
    for (uint64_t _inst_count{}; _inst_count < limit; _inst_count++)\
        salog::printFormat(salog::Info, "\t%llu. %#p = %#llx",\
            _inst_count, reinterpret_cast<uint32_t*>(origin) + _inst_count, *(reinterpret_cast<uint32_t*>(origin) + _inst_count))
#elif NDEBUG
#define DUMP_PRINT_INST(origin, limit)
#endif

void AArch64Patcher::emplaceMethod(const uintptr_t method, const uintptr_t super, 
    uint8_t instCount, bool runAfter)
{
    auto patcherCtx{reinterpret_cast<TrampolineContext*>(getNewTrampoline())};
    
    TEST_HOOK_CTX(patcherCtx);
    UPDATE_HOOK_CTX(patcherCtx, method, m_nemesis, instCount);
    
    auto patcherData{reinterpret_cast<uint32_t*>(patcherCtx->m_content.data())};
    auto const origin{reinterpret_cast<uint32_t*>(method)};

    if (runAfter) {
        for (uint32_t instIndex{}; instCount != instIndex; instIndex++) {
            *(patcherData + instIndex) = origin[instIndex];
        }
        // Save callee
        PLACE_FUNC_PROLOGUE(patcherData + instCount);

        IMM_FIX_PLACE(patcherData + 2 + instCount);
        MAKE_BRANCH(patcherData + 3 + instCount);
        FIX_BRANCH_LOCAL(patcherData + 4 + instCount, super);
        // Restore callee
        PLACE_FUNC_EPILOGUE(patcherData + 6 + instCount);

        PLACE_RET(patcherData + 7 + instCount);

    } else {
        // Save callee
        PLACE_FUNC_PROLOGUE(patcherData);
        // Putting our super function at the very beginning
        IMM_FIX_PLACE(patcherData + 2);
        MAKE_BRANCH(patcherData + 3);
        FIX_BRANCH_LOCAL(patcherData + 4, super);
        // Restore callee
        PLACE_FUNC_EPILOGUE(patcherData + 6);

        // Skip 2 bytes, used above
        for (uint32_t instIndex{}; instCount != instIndex; instIndex++) {
            *(patcherData + 7 + instIndex) = origin[instIndex];
        }

        PLACE_RET(patcherData + 7 + instCount);
    }

    unfuckPageRWX(reinterpret_cast<uintptr_t>(origin), PAGE_SIZE);

    IMM_FIX_PLACE(origin + 0);
    MAKE_BRANCH(origin + 1);
    // Placing our trampoline inside the class method/routine
    FIX_BRANCH_LOCAL(origin + 2, patcherData);

    CACHE_UPDATE_HOOK(patcherCtx);
    CACHE_UPDATE_ORIGIN(origin, instCount);

    DUMP_PRINT_INST(origin, patcherCtx->m_iCount + 1);
    DUMP_PRINT_INST(patcherData, PATCHER_MAX_INST);
}

void AArch64Patcher::placeHookAt(const uintptr_t method, const uintptr_t replace, uintptr_t* saveIn)
{
    salog::printFormat(salog::Info, "Hook: hooking member %#llx with %#llx method saving in %#llx", method, replace, saveIn);
    CLEAN_CONTEXT(*saveIn);
    TrampolineContext* const hookCtx{reinterpret_cast<TrampolineContext*>(getNewTrampoline())};
    TEST_HOOK_CTX(hookCtx);

    salog::printFormat(salog::Debug, "Hook: new trampoline allocated in %#p address", hookCtx);

    UPDATE_HOOK_CTX(hookCtx, method, m_nemesis, 0);
    // Doing the frame backup (we're divorciating now)
    
    auto originFunc{reinterpret_cast<uint32_t*>(method)};
    auto trContext{reinterpret_cast<uint32_t*>(hookCtx->m_content.data())};

    trContext[0] = originFunc[0];
    trContext[1] = originFunc[1];
    trContext[2] = originFunc[2];
    trContext[3] = originFunc[3];

    hookCtx->m_iCount = &originFunc[4] - &originFunc[0];

    // Installing our payload instruction, now we can overwrite the original method frame
    // making the page readable/writable and executable
    
    unfuckPageRWX(reinterpret_cast<uintptr_t>(originFunc), PAGE_SIZE);
    
    // ldr x17, #0x8 -> loading a 64 bit immediate value from offset PC + 0x8
    // br x17

    IMM_FIX_PLACE(originFunc);
    MAKE_BRANCH(originFunc + 1);

    FIX_BRANCH_LOCAL(originFunc + 2, replace);
    // At this point, the trampoline has the 4 instructions from the origin instruction
    // and the original instruction has a jump to your hook method

    IMM_FIX_PLACE(trContext + 4);
    MAKE_BRANCH(trContext + 5);

    FIX_BRANCH_LOCAL(trContext + 6, originFunc + 4);
    
    // Forcing the CPU to fetch the actual version for both operations 
    // Dumping the residual wrong instructions from the cache
    CACHE_UPDATE_HOOK(hookCtx);
    CACHE_UPDATE_ORIGIN(originFunc, hookCtx->m_iCount);

    DUMP_PRINT_INST(trContext, hookCtx->m_content.size());

    SAVE_CTX(*saveIn, trContext);

    salog::printFormat(salog::Info, "Hook: installed in addr %#llx by %#llx, (| %#llx | %u |)", method, replace, (uintptr_t)trContext & 0xffffffffff, hookCtx->m_iCount);
}
void AArch64Patcher::unfuckPageRWX(uintptr_t unfuckAddr, uint64_t region_size)
{
    const auto baseAddr{unfuckAddr & 0xfffffff000u};
    /* If page isn't aligned we can't change the permission for more than one page 
     * without break into multiples */

    const auto protect{PROT_READ | PROT_WRITE | PROT_EXEC};
    const auto pageSize{getpagesize()};
    
    auto countPages = [pageSize](const auto size) -> auto {
        uintptr_t count{size / pageSize};
        if (size % pageSize)
            count++;
        return count;
    };

    auto overflow{unfuckAddr & 0xffff ? 1 : 0};
    auto count{countPages(region_size) + overflow};

    salog::printFormat(salog::Info, "Hook: changing permission of %lu pages in %#llx base address", count, baseAddr);
    mprotect((void*)(baseAddr), count * pageSize, protect);
}

// While the game isn't loaded yet we need to fixes and hook some functions
namespace sapatch {
    using namespace saglobal;
    using namespace samimic;

    void applyOnGame()
    {
        g_patcherMicro = new AArch64Patcher();
        g_textureDatabase = new TextureDatabaseRuntime();
        // MenuItem_add is no longer present
        g_patcherMicro->placeHookAt(g_gameAddr + 0x358010, (uintptr_t)MainMenuScreen_AddAllItems, (uintptr_t*)&g_MainMenuScreen_AddAllItems);
        
        g_patcherMicro->placeHookAt(g_gameAddr + 0x55b668, (uintptr_t)CGame_InitializeRenderWare, (uintptr_t*)&g_CGame_InitializeRenderWare);
        
        g_patcherMicro->placeHookAt(g_gameAddr + 0x332040, (uintptr_t)NVThreadSpawnProc, (uintptr_t*)&g_NVThreadSpawnProc);
        
        g_patcherMicro->placeHookAt(g_gameAddr + 0x5c20a4, (uintptr_t)CClock_Update, (uintptr_t*)&g_CClock_Update);
        g_patcherMicro->emplaceMethod(g_gameAddr + 0x5cbaec, (uintptr_t)CEntity_UpdateRwFrame, 0x4, false);
    }

}
