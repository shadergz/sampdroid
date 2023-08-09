#pragma once

#include <log_client.h>
#include <patches_level.h>

// BIG - D32: 0x14000003
#define INC1_NEXT_PC(addr)\
    *reinterpret_cast<uint32_t*>((addr)) = __builtin_bswap32(0x03000014)

#define FIX2_BRANCH_LOCAL(at, by)\
    *reinterpret_cast<uint64_t**>((at)) = reinterpret_cast<uint64_t*>((uint64_t)((by)) & (uint64_t)-1)\

#define IMM1_FIX_LOAD_BRANCH(addr)\
    *reinterpret_cast<uint32_t*>((addr)) = __builtin_bswap32(0x51000058);\

#define RELOAD1_LINKER_R30(addr)\
    *reinterpret_cast<uint32_t*>((addr)) = __builtin_bswap32(0xfe0340f9)

// BIG - D32: 0x5800005d, &BY
#define IMM4_FIX_LINKER_R30(addr, by)\
    *reinterpret_cast<uint32_t*>((addr)) = __builtin_bswap32(0x5e000058);\
    INC1_NEXT_PC(addr + 1);\
    FIX2_BRANCH_LOCAL(addr + 2, by)

#define MAKE1_BRANCH_WITH_R17(addr)\
    *reinterpret_cast<uint32_t*>((addr)) = __builtin_bswap32(0x20021fd6)

// BIG: - D32: 0xd4200000
#define TRAP1_IN(addr)\
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
        SALOG_ASSERT(count >= 4 && count <= AArch64Patcher::PATCHER_MAX_INST, "At least four or more instructions are needed");\
    hook->m_iCount = count;\
    hook->m_id = m_randomDist(seed);\
    hook->m_source = method;\

#ifndef NDEBUG
#define DUMP_PRINT_INST(origin, limit)\
    salog::printFormat(salog::Debug, "Hook: dump of %llu from method %#p", limit, origin);\
    for (uint64_t _inst_count{}; _inst_count < limit; _inst_count++)\
        salog::printFormat(salog::Debug, "\t%llu. %#p = %#llx",\
            _inst_count, reinterpret_cast<uint32_t*>(origin) + _inst_count, *(reinterpret_cast<uint32_t*>(origin) + _inst_count))
#elif NDEBUG
#define DUMP_PRINT_INST(origin, limit)
#endif

#define CACHE_UPDATE_HOOK(hook)\
    __builtin___clear_cache((char*)hook->m_content.data(),\
        (char*)&hook->m_content + sizeof(hook->m_content))

#define CACHE_UPDATE_ORIGIN(origin, sz)\
    __builtin___clear_cache((char*)origin, (char*)&origin[sz])

