
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

void AArch64Patcher::emplaceMethod(const uintptr_t method, const uintptr_t super, 
    uint8_t instCount, bool runAfter)
{
    auto patcherCtx{reinterpret_cast<TrampolineContext*>(getNewTrampoline())};
    
    TEST_HOOK_CTX(patcherCtx);
    UPDATE_HOOK_CTX(patcherCtx, method, m_nemesis, instCount);
    
    auto patcherData{reinterpret_cast<uint32_t*>(patcherCtx->m_content.data())};
    auto const origin{reinterpret_cast<uint32_t*>(method)};

    for (uint32_t instIndex{}; instIndex != instCount; instIndex++) {
        const auto indexPtr{instIndex + (runAfter ? 0 : 9)};
        
        if (*(origin + instIndex) == __builtin_bswap32(0xd65f03c0))
            // Maybe we're running in a different context, where a ret or a branch may interfere and cause unexpected behavior
            std::runtime_error("Opcodes that modify the PC cannot be copied");

        *(patcherData + indexPtr) = *(origin + instIndex);
    }

    turnTextSegmentMutable(reinterpret_cast<uintptr_t>(origin), PAGE_SIZE);

    IMM1_FIX_LOAD_BRANCH(origin + 0);
    MAKE1_BRANCH_WITH_R17(origin + 1);

    // Placing our trampoline inside the class method/routine
    FIX2_BRANCH_LOCAL(origin + 2, patcherData);
    RELOAD1_LINKER_R30(origin + 4);
    
    if (runAfter) {
        // FUNCTION CODE HERE

        // Save callee
    IMM4_FIX_LINKER_R30(patcherData + instCount, patcherData + 8 + instCount);

        IMM1_FIX_LOAD_BRANCH(patcherData + 4 + instCount);
        MAKE1_BRANCH_WITH_R17(patcherData + 5 + instCount);
        FIX2_BRANCH_LOCAL(patcherData + 6 + instCount, super);
        // Restore callee

    IMM1_FIX_LOAD_BRANCH(patcherData + 8 + instCount);
        
        MAKE1_BRANCH_WITH_R17(patcherData + 9 + instCount);
        FIX2_BRANCH_LOCAL(patcherData + 10 + instCount, origin + instCount - 1);

    } else {
        // Save callee

        // Putting our super function at the very beginning
    IMM4_FIX_LINKER_R30(patcherData, patcherData + 8 + instCount);

        IMM1_FIX_LOAD_BRANCH(patcherData + 4);
        MAKE1_BRANCH_WITH_R17(patcherData + 5);
        FIX2_BRANCH_LOCAL(patcherData + 6, super);
        // Restore callee

        // FUNCTION CODE HERE
    IMM1_FIX_LOAD_BRANCH(patcherData + 8 + instCount);

        MAKE1_BRANCH_WITH_R17(patcherData + 9 + instCount);
        FIX2_BRANCH_LOCAL(patcherData + 10 + instCount, origin + instCount - 1);
    }

    CACHE_UPDATE_HOOK(patcherCtx);
    CACHE_UPDATE_ORIGIN(origin, instCount);

    DUMP_PRINT_INST(origin, patcherCtx->m_iCount);
    DUMP_PRINT_INST(patcherData, PATCHER_MAX_INST);
}

void AArch64Patcher::placeHookAt(const uintptr_t method, const uintptr_t replace, uintptr_t* saveIn)
{
    salog::printFormat(salog::Info, "Hook: hooking member %#llx with %#llx function, and saving in %#llx", method, replace, saveIn);
    CLEAN_CONTEXT(*saveIn);
    TrampolineContext* const hookCtx{reinterpret_cast<TrampolineContext*>(getNewTrampoline())};
    TEST_HOOK_CTX(hookCtx);

    salog::printFormat(salog::Debug, "Hook: new trampoline allocated in %#llx address", hookCtx);

    UPDATE_HOOK_CTX(hookCtx, method, m_nemesis, 0);
    // Doing the frame backup (we're divorciating now)
    
    auto originFunc{reinterpret_cast<uint32_t*>(method)};
    auto trContext{reinterpret_cast<uint32_t*>(hookCtx->m_content.data())};

    {
        int nextToCopy{};

        trContext[nextToCopy++] = originFunc[0];
        trContext[nextToCopy++] = originFunc[1];
        trContext[nextToCopy++] = originFunc[2];
        trContext[nextToCopy++] = originFunc[3];

        hookCtx->m_iCount = &originFunc[nextToCopy] - &originFunc[0];

        // At this point, the trampoline has the 4 instructions from the origin instruction
        // and the original instruction has a jump to your hook method

        if (originFunc[nextToCopy] == __builtin_bswap32(0xd65f03c0)) {
            throw std::runtime_error("originFunc[nextToCopy] is an instruction that is used to return from a subroutine");
        }

        IMM1_FIX_LOAD_BRANCH(trContext + nextToCopy);
        MAKE1_BRANCH_WITH_R17(trContext + 1 + nextToCopy);
        FIX2_BRANCH_LOCAL(trContext + 2 + nextToCopy, originFunc + nextToCopy);
    }

    // Installing our payload instruction, now we can overwrite the original method frame
    // making the page readable/writable and executable
    
    turnTextSegmentMutable(reinterpret_cast<uintptr_t>(originFunc), PAGE_SIZE);
    
    // LDR x17, #0x8 -> loading a 64 bit immediate value from offset PC + 0x4
    // BR x17

    IMM1_FIX_LOAD_BRANCH(originFunc);
    MAKE1_BRANCH_WITH_R17(originFunc + 1);
    FIX2_BRANCH_LOCAL(originFunc + 2, replace);
    
    // Forcing the CPU to fetch the actual version for both operations 
    // Dumping the residual wrong instructions from the cache
    CACHE_UPDATE_HOOK(hookCtx);
    CACHE_UPDATE_ORIGIN(originFunc, hookCtx->m_iCount);

    DUMP_PRINT_INST(originFunc, hookCtx->m_iCount);
    DUMP_PRINT_INST(trContext, PATCHER_MAX_INST);

    SAVE_CTX(*saveIn, trContext);

    salog::printFormat(salog::Info, "Hook: installed in address [%#llx] by @%#llx@, (| %#llx | %u |)", method, replace, (uintptr_t)trContext & 0xffffffffff, hookCtx->m_iCount);
}
void AArch64Patcher::turnTextSegmentMutable(uintptr_t textPage, uint64_t regionSize)
{
    const auto baseAddr{textPage & 0xfffffff000u};
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

    auto overflow{textPage & 0xffff ? 1 : 0};
    auto count{countPages(regionSize) + overflow};

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
        g_patcherMicro->placeHookAt(g_gameAddr + 0x55b668, (uintptr_t)CGame_InitialiseRenderWare, (uintptr_t*)&g_CGame_InitialiseRenderWare);
        g_patcherMicro->placeHookAt(g_gameAddr + 0x332040, (uintptr_t)NVThreadSpawnProc, (uintptr_t*)&g_NVThreadSpawnProc);
        g_patcherMicro->placeHookAt(g_gameAddr + 0x4c20a4, (uintptr_t)CClock_Update, (uintptr_t*)&g_CClock_Update);
        
        g_patcherMicro->emplaceMethod(g_gameAddr + 0x4cbb00, (uintptr_t)CEntity_UpdateRpHAnim, 0x5, true);
        g_patcherMicro->emplaceMethod(g_gameAddr + 0x51f6f0, (uintptr_t)CHUD_Draw, 0x5, false);
    
    }

}
