#pragma once
#include <malloc.h>

#include <android/log.h>
#include <cstdio>
#include <utility>

#include <fmt/std.h>

int mtmprintf(int prio, const char* __restrict format, ...);

int mtmputs(int prio, const char* __restrict msgStr);

template<typename... Args>
auto mtmcout(int prio, fmt::format_string<Args...> fmt, Args&&... args) {
    uint64_t fioAllocated{};
    char* fioPtr{};

    // This FILE IO is dynamic allocated and grows up as required
    FILE* fio{open_memstream(&fioPtr, (size_t*)&fioAllocated)};
    fmt::print(fio, fmt, std::forward<Args>(args)...);
    fclose(fio);
    // At this point, a null byte has wrote inside of fio string

    const auto out{mtmputs(prio, fioPtr)};
    free(fioPtr);
    return out;
}
