#include <log_client.h>
#include <patches_level.h>

#pragma once

#define ___fix_branch_location(at, by)\
    *(uint64_t**)(at) = (uint64_t*)(reinterpret_cast<uintptr_t>(by) & 0xffffffffff)

#define __imm_fix_place(addr)\
    *addr = __builtin_bswap32(0x51000058);

#define __make_branch(addr)\
    *addr = __builtin_bswap32(0x20021fd6);

#define __test_hook(hook)\
    if (!hook)\
        std::terminate();\

#define __clean_context(output)\
    output = {}

#define __save_output(output, ctx)\
    output = reinterpret_cast<uintptr_t>(ctx)

#define __save_hook(hook, method, seed, count)\
    if (count)\
        SALOG_ASSERT(count >= 4, "At least four instruction are needed");\
    hook->m_instCount = count;\
    hook->m_id = m_randomDist(seed);\
    hook->m_source = method;\

#define __cache_hook_update(hook)\
    __builtin___clear_cache((char*)hook->m_content.data(),\
        (char*)&hook->m_content + sizeof(hook->m_content));

#define __cache_origin_drop(origin, sz)\
    __builtin___clear_cache((char*)origin, (char*)&origin[sz]);

