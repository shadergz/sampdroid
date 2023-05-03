#pragma once
#include <malloc.h>

#include <android/log.h>
#include <cstdio>
#include <utility>

#include <fmt/std.h>

namespace Mt4Global {
extern const char* g_logTag;
}

namespace Mt4Log {
int printflike(int prio, const char* __restrict format, ...);
int putslike(int prio, const char* __restrict msg_str);

template<typename... Args>
auto coutlike(int prio, fmt::format_string<Args...> fmt, Args&&... args) 
{
    uint64_t fioAllocated{};
    char* fioPtr{};

    // this FILE IO is dynamic allocated and grows up as required
    FILE* fio{open_memstream(&fioPtr, (size_t*)&fioAllocated)};
    fmt::print(fio, fmt, std::forward<Args>(args)...);
    fclose(fio);
    // at this point, a null byte has wrote inside of fio string

    const auto out{Mt4Log::putslike(prio, fioPtr)};
    free(fioPtr);
    return out;
}

void assertAbort(const char* cond, const char* fileName, 
    int line, const char* __restrict format, ...);

}

#define MT4LOG_RUNTIME_ASSERT(cond, fmt, ...)\
    if (!(cond))\
        Mt4Log::assertAbort(#cond, __FILE__, __LINE__, fmt, #__VA_ARGS__)


