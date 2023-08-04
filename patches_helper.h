#include <log_client.h>
#include <patches_level.h>

#pragma once

#define FIX_BRANCH_LOCAL(at, by)\
    *reinterpret_cast<uint64_t**>((at)) = reinterpret_cast<uint64_t*>((uint64_t)((by)) & (uint64_t)-1)\

#define IMM_FIX_PLACE(addr)\
    *reinterpret_cast<uint32_t*>((addr)) = __builtin_bswap32(0x51000058)

#define MAKE_BRANCH(addr)\
    *reinterpret_cast<uint32_t*>((addr)) = __builtin_bswap32(0x20021fd6)

#define PLACE_FUNC_PROLOGUE(addr)\
    *reinterpret_cast<uint32_t*>((addr)) = __builtin_bswap32(0xa9bf7bfd);\
    *reinterpret_cast<uint32_t*>((addr + 1)) = __builtin_bswap32(0x910003fd)\

#define PLACE_FUNC_EPILOGUE(addr)\
    *reinterpret_cast<uint32_t*>((addr)) = __builtin_bswap32(0xa8c17bfd)\

#define PLACE_RET(addr)\
    *reinterpret_cast<uint32_t*>((addr)) = __builtin_bswap32(0xd65f03c0);

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



