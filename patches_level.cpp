
#include <cstdint>

#include <unistd.h>
#include <sys/user.h>
#include <sys/mman.h>

#include <patches_level.h>
#include <outside.h>

#include <menu_handler.h>
#include <nv_threads.h>

AArch64_Patcher* g_patcher_micro;
extern uintptr_t g_game_addr;

enum Macro_BranchMode {};

#pragma pack(push, 1)

struct Trampoline_Data {
    uint16_t m_id;
    uintptr_t m_source;
    // Count of instructions replaced inside the origin and placed inside of our trampoline
    uint8_t m_inst_count;
    uint8_t m_mode;

    char m_origin_symbolname[AArch64_Patcher::PATCHER_SYMBOL_NAME];
    uint8_t m_tr_data[sizeof(uint32_t) * AArch64_Patcher::PATCHER_MAX_INST];
};

#pragma pack(pop)

static constexpr uint8_t PATCHER_FRAME_GOBACK = offsetof(Trampoline_Data, m_tr_data);

static_assert(sizeof(Trampoline_Data) == AArch64_Patcher::PATCHER_HOOK_SIZE,
    "Trampoline struct data size is invalid and must be fixed!");

void AArch64_Patcher::placeHookAt(const char* sb_name, const uintptr_t method, 
    const uintptr_t replace, uintptr_t* save_in)
{
    auto sb_size{strlen(sb_name)};
    if (sb_size > sizeof (Trampoline_Data::m_origin_symbolname)) {
        mtmprintf(ANDROID_LOG_ERROR, "Symbol name %s is langer than the symbol name space!", sb_name);
        *save_in = 0; return;
    }

    mtmprintf(ANDROID_LOG_INFO, 
        "Hooking function (%s) %#llx with %#llx method, "
            "saving in %#llx", sb_name, method, replace, save_in);
    auto tr_data{reinterpret_cast<Trampoline_Data*>(getNewTrampoline())};
    mtmprintf(ANDROID_LOG_INFO, "New trampoline allocated in %p\n", tr_data);
    
    if (!tr_data) { *save_in = 0; return; }
    
    static uint16_t tramp_id{};
    
    tr_data->m_id = tramp_id++;
    tr_data->m_source = method;
    // 0 means an invalid value!

    strncpy(tr_data->m_origin_symbolname, sb_name, 
        sizeof (tr_data->m_origin_symbolname));
    MTM_RUNTIME_ASSERT((uintptr_t)tr_data->m_tr_data == (uintptr_t)tr_data + PATCHER_FRAME_GOBACK, 
        "PATCHER_FRAME_GOBACK isn't indexing the trampoline data as expected, please fix now!");
    
    // doing the frame backup (we're divorciating now)
    auto origin_func{(uint32_t*)method};
    *(uint32_t*)(tr_data->m_tr_data+0) = origin_func[0];
    *(uint32_t*)(tr_data->m_tr_data+4) = origin_func[1];
    *(uint32_t*)(tr_data->m_tr_data+8) = origin_func[2];
    *(uint32_t*)(tr_data->m_tr_data+12) = origin_func[3];

    tr_data->m_inst_count = &origin_func[4] - &origin_func[0];

    // Installing our payload instructions

    /* Now we can overwrite the original method frame
     * making the page readable/writable and executable */
    unfuckPageRWX((uintptr_t)origin_func, PAGE_SIZE);
    // ldr x17, #0x8 -> loading a 64 bit immediate value from offset PC + 0x8
    *(origin_func+0) = __builtin_bswap32(0x51000058);
    // br x17
    *(origin_func+1) = __builtin_bswap32(0x20021fd6); 

    *(uint64_t**)(origin_func+2) = (uint64_t*)(replace & (uint64_t)-1);
    /* At this point, the trampoline has the 4 instructions from the origin instruction
     * and the original instruction has a jump to your hook method */
    *(uint64_t*)(tr_data->m_tr_data+16) = (uint64_t)__builtin_bswap64(0x5100005820021fd6);
    *(uint64_t**)(tr_data->m_tr_data+24) = (uint64_t*)((uint64_t)(origin_func+0x4) & (uint64_t)-1);    
    // Forcing the CPU to fetch the actual version for both operations 
    // Dumping the residual wrong instructions from the cache
    __builtin___clear_cache((char*)origin_func, (char*)&origin_func[4]);
    __builtin___clear_cache((char*)tr_data->m_tr_data, 
        (char*)&tr_data->m_tr_data[sizeof(tr_data->m_tr_data)]);

    mtmprintf(ANDROID_LOG_INFO, 
        "Hook on addr %#llx successful installed by %#llx, (| %#llx | %u |)",
        method, replace, (uintptr_t)tr_data->m_tr_data & 0xffffffffff, 
        tr_data->m_inst_count);

    *save_in = (uintptr_t)(tr_data->m_tr_data);
}
void AArch64_Patcher::unfuckPageRWX(uintptr_t unfuck_addr, uint64_t region_size)
{
    const auto base_addr{unfuck_addr & 0xfffffff000u};
    /* If page isn't aligned we can't change the permission for more than once page 
     * without break into multiples */

    const auto protect{PROT_READ|PROT_WRITE|PROT_EXEC};
    const auto page_size{getpagesize()};
    
    auto count_pages = [page_size](const auto size) -> auto {
        uintptr_t count{size / page_size};
        if (size % page_size)
            count++;
        return count;
    };

    auto overflow = unfuck_addr & 0xffff ? 1 : 0;
    auto count{count_pages(region_size) + overflow};

    mtmprintf(ANDROID_LOG_INFO, 
        "Changing permission of %lu pages in %#llx base address", count, base_addr);
    mprotect((void*)(base_addr), count * page_size, protect);
}

// While the game is loading we need to fixes and hook some functions
void applyOnGamePatches()
{
    g_patcher_micro = new AArch64_Patcher();

    // MenuItem_add is no longer present
    g_patcher_micro->placeHookAt("AddAllItems", g_game_addr+0x358010, (uintptr_t)(MainMenuScreen_AddAllItems_HOOK), (uintptr_t*)(&MainMenuScreen_AddAllItems));
    g_patcher_micro->placeHookAt("NVThreadSpawnProc", g_game_addr+0x332040, (uintptr_t)NVThreadSpawnProc_HOOK, (uintptr_t*)&NVThreadSpawnProc);


}
