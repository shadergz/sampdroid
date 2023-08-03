#include <log_client.h>
#include <patches_level.h>

#pragma once

#define FIX_BRANCH_LOCAL(at, by)\
    *(uint64_t**)(at) = (uint64_t*)(reinterpret_cast<uintptr_t>(by) & 0xffffffffff)

#define IMM_FIX_PLACE(addr)\
    *addr = __builtin_bswap32(0x51000058);

#define MAKE_BRANCH(addr)\
    *addr = __builtin_bswap32(0x20021fd6);

#define TEST_HOOK_CTX(hook)\
    if (!hook)\
        std::terminate();\

#define CLEAN_CONTEXT(output)\
    output = {}

#define SAVE_CTX(output, ctx)\
    output = reinterpret_cast<uintptr_t>(ctx)

#define UPDATE_HOOK_CTX(hook, method, seed, count)\
    if (count)\
        SALOG_ASSERT(count >= 4, "At least four instruction are needed");\
    hook->m_instCount = count;\
    hook->m_id = m_randomDist(seed);\
    hook->m_source = method;\

#define CACHE_UPDATE_HOOK(hook)\
    __builtin___clear_cache((char*)hook->m_content.data(),\
        (char*)&hook->m_content + sizeof(hook->m_content));

#define CACHE_UPDATE_ORIGIN(origin, sz)\
    __builtin___clear_cache((char*)origin, (char*)&origin[sz]);

