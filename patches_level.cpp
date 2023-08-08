
#include <cstring>
#include <array>

#include <exception>
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

//#ifndef NDEBUG
#define DUMP_PRINT_INST(origin, limit)\
    salog::printFormat(salog::Info, "Hook: dump of %llu from method %#p", limit, origin);\
    for (uint64_t _inst_count{}; _inst_count < limit; _inst_count++)\
        salog::printFormat(salog::Info, "\t%llu. %#p = %#llx",\
            _inst_count, reinterpret_cast<uint32_t*>(origin) + _inst_count, *(reinterpret_cast<uint32_t*>(origin) + _inst_count))
//#elif NDEBUG
//#define DUMP_PRINT_INST(origin, limit)
//#endif

void AArch64Patcher::emplaceMethod(const uintptr_t method, const uintptr_t super, 
    uint8_t instCount, bool runAfter)
{
    auto patcherCtx{reinterpret_cast<TrampolineContext*>(getNewTrampoline())};
    
    TEST_HOOK_CTX(patcherCtx);
    UPDATE_HOOK_CTX(patcherCtx, method, m_nemesis, instCount);
    
    auto patcherData{reinterpret_cast<uint32_t*>(patcherCtx->m_content.data())};
    auto const origin{reinterpret_cast<uint32_t*>(method)};

    for (uint32_t instIndex{}; instCount != instIndex; instIndex++) {
        const auto indexPtr{instIndex + (runAfter ? 0 : 12)};
        if (*(origin + indexPtr) == __builtin_bswap32(0xd65f03c0))
            // Maybe we're running in a different context, where a ret or a branch may interfere and cause unexpected behavior
            std::runtime_error("Opcodes that modify the PC cannot be copied");

        *(patcherData + indexPtr) = *(origin + instIndex);
    }

    unfuckPageRWX(reinterpret_cast<uintptr_t>(origin), PAGE_SIZE);

    IMM2_FIX_BRANCH(origin + 0);
    // Placing our trampoline inside the class method/routine
    FIX2_BRANCH_LOCAL(origin + 2, patcherData);
    MAKE1_BRANCH_WITH_R17(origin + 4);

    if (runAfter) {
        // FUNCTION CODE HERE

        // Save callee
        PLACE2_FUNC_PROLOGUE(patcherData + instCount);
        IMM4_FIX_LINKER_R30(patcherData + 2 + instCount, patcherData + 9);

        IMM2_FIX_BRANCH(patcherData + 6 + instCount);
        MAKE1_BRANCH_WITH_R17(patcherData + 8 + instCount);
        FIX2_BRANCH_LOCAL(patcherData + 9 + instCount, super);
        // Restore callee
        PLACE1_FUNC_EPILOGUE(patcherData + 11 + instCount);

        IMM2_FIX_BRANCH(patcherData + 12 + instCount);
        FIX2_BRANCH_LOCAL(patcherData + 14 + instCount, origin + instCount);
        MAKE1_BRANCH_WITH_R17(patcherData + 16 + instCount);

    } else {
        // Save callee
        PLACE2_FUNC_PROLOGUE(patcherData);
        // Putting our super function at the very beginning
        IMM4_FIX_LINKER_R30(patcherData + 2, patcherData + 11);

        IMM2_FIX_BRANCH(patcherData + 6);
        FIX2_BRANCH_LOCAL(patcherData + 8, super);
        MAKE1_BRANCH_WITH_R17(patcherData + 10);
        // Restore callee
        PLACE1_FUNC_EPILOGUE(patcherData + 11);

        // FUNCTION CODE HERE

        IMM2_FIX_BRANCH(patcherData + 12 + instCount);

        FIX2_BRANCH_LOCAL(patcherData + 14 + instCount, origin + instCount);
        MAKE1_BRANCH_WITH_R17(patcherData + 16 + instCount);
    }

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

    int nextToCopy{};

    trContext[nextToCopy++] = originFunc[0];
    trContext[nextToCopy++] = originFunc[1];
    trContext[nextToCopy++] = originFunc[2];
    trContext[nextToCopy++] = originFunc[3];

    if (originFunc[4] != __builtin_bswap32(0xd65f03c0)) {
        trContext[nextToCopy++] = originFunc[4];
    } else {
        throw std::runtime_error("originFunc[nextToCopy] is an instruction that is used to return from a subroutine");
    }

    hookCtx->m_iCount = &originFunc[nextToCopy] - &originFunc[0];

    // Installing our payload instruction, now we can overwrite the original method frame
    // making the page readable/writable and executable
    
    unfuckPageRWX(reinterpret_cast<uintptr_t>(originFunc), PAGE_SIZE);
    
    // LDR x17, #0x8 -> loading a 64 bit immediate value from offset PC + 0x4
    // BR x17

    IMM2_FIX_BRANCH(originFunc);
    FIX2_BRANCH_LOCAL(originFunc + 2, replace);
    MAKE1_BRANCH_WITH_R17(originFunc + 4);

    // At this point, the trampoline has the 4 instructions from the origin instruction
    // and the original instruction has a jump to your hook method

    IMM2_FIX_BRANCH(trContext + nextToCopy);
    FIX2_BRANCH_LOCAL(trContext + 2 + nextToCopy, originFunc + nextToCopy);

    MAKE1_BRANCH_WITH_R17(trContext + 4 + nextToCopy);
    
    // Forcing the CPU to fetch the actual version for both operations 
    // Dumping the residual wrong instructions from the cache
    CACHE_UPDATE_HOOK(hookCtx);
    CACHE_UPDATE_ORIGIN(originFunc, hookCtx->m_iCount);

    // DUMP_PRINT_INST(originFunc, hookCtx->m_iCount);
    // DUMP_PRINT_INST(trContext, PATCHER_MAX_INST);

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
        
        g_patcherMicro->placeHookAt(g_gameAddr + 0x65b668, (uintptr_t)CGame_InitializeRenderWare, (uintptr_t*)&g_CGame_InitializeRenderWare);
        
        g_patcherMicro->placeHookAt(g_gameAddr + 0x332040, (uintptr_t)NVThreadSpawnProc, (uintptr_t*)&g_NVThreadSpawnProc);
        
        g_patcherMicro->placeHookAt(g_gameAddr + 0x5c20a4, (uintptr_t)CClock_Update, (uintptr_t*)&g_CClock_Update);
        // g_patcherMicro->emplaceMethod(g_gameAddr + 0x5cbb00, (uintptr_t)CEntity_UpdateRpHAnim, 0x4, false);
    }

}
