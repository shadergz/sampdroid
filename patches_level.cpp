#include <cstring>
#include <array>

#include <patches_level.h>
#include <texture_runtime.h>

#include <menu_handler.h>
#include <nv_threads.h>

namespace saglobal {
    AArch64Patcher* g_patcherMicro;
    extern uintptr_t g_gameAddr;
}

enum MicroBranchMode {};

#pragma pack(push, 1)
struct TrampolineContext {
    uint16_t m_id;
    uintptr_t m_source;
    // Count of instructions replaced inside the origin and placed inside of our trampoline
    uint8_t m_instCount;
    uint8_t m_mode;

    std::array<char, AArch64Patcher::PATCHER_SYMBOL_NAME> m_symbolName;
    std::array<uint8_t, sizeof(uint32_t) * AArch64Patcher::PATCHER_MAX_INST> m_content;
};

#pragma pack(pop)
static constexpr uint8_t PATCHER_FRAME_GOBACK{offsetof(TrampolineContext, m_content)};
static_assert(sizeof(TrampolineContext) == AArch64Patcher::PATCHER_HOOK_SIZE,
    "Trampoline struct data size is invalid and must be fixed!");

void AArch64Patcher::placeHookAt(const char* sbName, const uintptr_t method,
    const uintptr_t replace, uintptr_t* saveIn)
{
    static const auto symbolSize{AArch64Patcher::PATCHER_SYMBOL_NAME};

    salog::printFormat(ANDROID_LOG_INFO, "Hooking function (%s) %#llx with %#llx method, "
        "saving in %#llx", sbName, method, replace, saveIn);

    *saveIn = {};

    if (strlen(sbName) > symbolSize) {
        salog::printFormat(ANDROID_LOG_ERROR, "Symbol name %s is larger than the symbol name space!", sbName);
        return;
    }
    auto hookableCtx{reinterpret_cast<TrampolineContext*>(getNewTrampoline())};
    if (!hookableCtx)
        return;
    
    salog::printFormat(ANDROID_LOG_INFO, "New trampoline allocated in %p\n", hookableCtx);
    static uint16_t trampId{};
    
    hookableCtx->m_id = trampId++;
    hookableCtx->m_source = method;
    // 0 means an invalid value!

    std::strncpy(hookableCtx->m_symbolName.data(), sbName, symbolSize);
    SALOG_ASSERT(hookableCtx->m_content.data() == reinterpret_cast<uint8_t*>(hookableCtx) + PATCHER_FRAME_GOBACK,
        "PATCHER_FRAME_GOBACK isn't indexing the trampoline data as expected please fix now!");
    
    // Doing the frame backup (we're divorciating now)
    auto originFunc{reinterpret_cast<uint32_t*>(method)};
    auto trContext{reinterpret_cast<uint8_t*>(hookableCtx->m_content.data())};

    *(uint32_t*)(trContext + 0) = originFunc[0];
    *(uint32_t*)(trContext + 4) = originFunc[1];
    *(uint32_t*)(trContext + 8) = originFunc[2];
    *(uint32_t*)(trContext + 12) = originFunc[3];

    hookableCtx->m_instCount = &originFunc[4] - &originFunc[0];

    // Installing our payload instruction, now we can overwrite the original method frame
    // making the page readable/writable and executable
    unfuckPageRWX(reinterpret_cast<uintptr_t>(originFunc), PAGE_SIZE);
    // ldr x17, #0x8 -> loading a 64 bit immediate value from offset PC + 0x8
    *(originFunc + 0) = __builtin_bswap32(0x51000058);
    // br x17
    *(originFunc + 1) = __builtin_bswap32(0x20021fd6);

    *(uint64_t**)(originFunc + 2) = (uint64_t*)(replace & (uint64_t)-1);
    // At this point, the trampoline has the 4 instructions from the origin instruction
    // and the original instruction has a jump to your hook method
    *(uint64_t*)(trContext + 16) = (uint64_t)__builtin_bswap64(0x5100005820021fd6);
    *(uint64_t**)(trContext + 24) = (uint64_t*)((uint64_t)(originFunc + 0x4) & (uint64_t)-1);
    // Forcing the CPU to fetch the actual version for both operations 
    // Dumping the residual wrong instructions from the cache
    __builtin___clear_cache((char*)originFunc, (char*)&originFunc[4]);
    __builtin___clear_cache((char*)hookableCtx->m_content.data(),
        (char*)&hookableCtx->m_content + sizeof(hookableCtx->m_content));
    *saveIn = (uintptr_t)(trContext);

    salog::printFormat(ANDROID_LOG_INFO, "Hook on addr %#llx successful installed by %#llx, (| %#llx | %u |)",
        method, replace, (uintptr_t)trContext & 0xffffffffff, hookableCtx->m_instCount);
}
void AArch64Patcher::unfuckPageRWX(uintptr_t unfuckAddr, uint64_t region_size)
{
    const auto baseAddr{unfuckAddr & 0xfffffff000u};
    /* If page isn't aligned we can't change the permission for more than once page 
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

    salog::printFormat(ANDROID_LOG_INFO, "Changing permission of %lu pages in %#llx base address", count, baseAddr);
    mprotect((void*)(baseAddr), count * pageSize, protect);
}

// While the game is loading we need to fixes and hook some functions
namespace sapatch {

    using namespace saglobal;

    void applyOnGame()
    {
        g_patcherMicro = new AArch64Patcher();
        g_textureDatabase = new TextureDatabaseRuntime();

        // MenuItem_add is no longer present
        g_patcherMicro->placeHookAt("AddAllItems", g_gameAddr + 0x358010,
            (uintptr_t)samimic::MainMenuScreen_AddAllItems,
            (uintptr_t*)&saglobal::g_MainMenuScreen_AddAllItems);
    
        g_patcherMicro->placeHookAt("NVThreadSpawnProc", g_gameAddr + 0x332040,
            (uintptr_t)samimic::NVThreadSpawnProc,
            (uintptr_t*)&saglobal::g_NVThreadSpawnProc);
    }

}
