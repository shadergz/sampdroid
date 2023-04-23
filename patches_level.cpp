
#include <android/log.h>
#include <patches_level.h>
#include <menu_handler.h>
#include <outside.h>

#include <stdint.h>
#include <unistd.h>
#include <sys/mman.h>

AArch64_Patcher* g_patcher_micro;
extern uintptr_t g_game_addr;

enum Macro_BranchMode {};

#pragma pack(push, 1)
struct Trampoline_Data {
    uint16_t m_id;
    uintptr_t m_source;
    // Count of instruction replace inside the origin and placed inside of trampoline
    uint8_t m_inst_count;
    uint8_t m_mode;

    char m_origin_symbolname[AArch64_Patcher::PATCHER_SYMBOL_NAME];
    uint8_t m_tr_data[sizeof(uint32_t) * AArch64_Patcher::PATCHER_MAX_INST];
};
#pragma pack(pop)

static constexpr uint8_t PATCHER_FRAME_GOBACK = offsetof(Trampoline_Data, m_tr_data);

static_assert(sizeof(Trampoline_Data) == AArch64_Patcher::PATCHER_HOOK_SIZE,
    "trampoline struct data size is invalid and must be fixed!");

void AArch64_Patcher::replaceMethod(const char* sb_name, const uintptr_t method, 
    const uintptr_t replace, uintptr_t* save_in)
{
    if (strlen(sb_name) > sizeof (Trampoline_Data::m_origin_symbolname)) {
        mtmprintf(ANDROID_LOG_ERROR, "symbol name %s is langer than the symbol name space!", sb_name);
        *save_in = 0; return;
    }

    mtmprintf(ANDROID_LOG_INFO, 
        "hooking function (%s) %#lx with %#lx method, "
            "saving in %#lx", sb_name, method, replace, save_in);
    auto tr_data{reinterpret_cast<Trampoline_Data*>(getNewTrampoline())};
    if (!tr_data) { *save_in = 0; return; }
    static uint16_t tramp_id{};
    tr_data->m_id = tramp_id++;
    tr_data->m_source = method;
    // 0 means an invalid value!
    tr_data->m_inst_count = 0;

    strncpy(tr_data->m_origin_symbolname, sb_name, 
        sizeof (tr_data->m_tr_data));

    if ((uintptr_t)tr_data->m_tr_data != (uintptr_t)tr_data + PATCHER_FRAME_GOBACK) {
    __android_log_assert(
        "(uintptr_t)tr_data->m_tr_data != (uintptr_t)tr_data + PATCHER_FRAME_GOBACK",
        g_mtmTag,
        "PATCHER_FRAME_GOBACK isn't indexing the trampoline data as expected, please fix now!");
    }

    mtmprintf(ANDROID_LOG_INFO, 
        "function %s with addr %#lx hooked by %#lx, (trampoline: %#lx | %u |)",
        sb_name, method, replace, tr_data->m_tr_data, tr_data->m_inst_count);

    *save_in = (uintptr_t)(tr_data->m_tr_data);
}
void AArch64_Patcher::unfuckPageRWX(uintptr_t unfuck_addr, uint64_t region_size)
{
    const auto base_addr{unfuck_addr & 0xfffff000u};
    // if page isn't aligned we can't change the permission for more than once page 
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
    g_patcher_micro->replaceMethod("MainMenuScreen::AddAllItems", 
        g_game_addr+0x358010, (uintptr_t)(MainMenuScreen_AddAllItems_HOOK), (uintptr_t*)(&MainMenuScreen_AddAllItems));

}