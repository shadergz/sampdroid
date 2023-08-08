#include <log_client.h>
#include <patches_level.h>

#pragma once

// BID - D32: 0x14000003
#define INC1_NEXT_PC(addr)\
    *reinterpret_cast<uint32_t*>((addr)) = __builtin_bswap32(0x03000014)

#define FIX2_BRANCH_LOCAL(at, by)\
    *reinterpret_cast<uint64_t**>((at)) = reinterpret_cast<uint64_t*>((uint64_t)((by)) & (uint64_t)-1)\

#define IMM2_FIX_BRANCH(addr)\
    *reinterpret_cast<uint32_t*>((addr)) = __builtin_bswap32(0x51000058);\
    INC1_NEXT_PC(addr + 1)

// BIG - D32: 0x5800005d, &BY
#define IMM4_FIX_LINKER_R30(addr, by)\
    *reinterpret_cast<uint32_t*>((addr)) = __builtin_bswap32(0x5e000058);\
    INC1_NEXT_PC(addr + 1);\
    FIX2_BRANCH_LOCAL(addr + 2, by)

#define MAKE1_BRANCH_WITH_R17(addr)\
    *reinterpret_cast<uint32_t*>((addr)) = __builtin_bswap32(0x20021fd6)

// BIG - D32: 0xfd7bbfa9, 0xfd030091. W64: 0xfd030091fd7bbfa9
// LITTLE - D32: 0xa9bf7bfd, 0x910003fd

#define PLACE2_FUNC_PROLOGUE(addr)\
    *reinterpret_cast<uint64_t*>((addr)) = __builtin_bswap64(0xfd7bbfa9fd030091)

// BIG - D32: 0xa9417bfd

#define PLACE1_FUNC_EPILOGUE(addr)\
    *reinterpret_cast<uint32_t*>((addr)) = __builtin_bswap32(0xa8c17bfd)

// BIG: - W32: 0xd4200000
#define PLACE1_BREAKPOINT(addr)\
    *reinterpret_cast<uint32_t*>((addr)) = __builtin_bswap32(0x000020d4)

#define TEST_HOOK_CTX(hook)\
    if (!hook)\
        std::terminate();\

#define CLEAN_CONTEXT(output)\
    output = {}

#define SAVE_CTX(output, ctx)\
    output = reinterpret_cast<uintptr_t>(ctx)

#define UPDATE_HOOK_CTX(hook, method, seed, count)\
    if (count)\
        SALOG_ASSERT(count >= 4, "At least four instructions are needed");\
    hook->m_iCount = count;\
    hook->m_id = m_randomDist(seed);\
    hook->m_source = method;\

#define CACHE_UPDATE_HOOK(hook)\
    __builtin___clear_cache((char*)hook->m_content.data(),\
        (char*)&hook->m_content + sizeof(hook->m_content))

#define CACHE_UPDATE_ORIGIN(origin, sz)\
    __builtin___clear_cache((char*)origin, (char*)&origin[sz])



