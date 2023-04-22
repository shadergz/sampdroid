#pragma once
#include <malloc.h>

#include <android/log.h>
#include <cstdio>
#include <utility>

#include <fmt/std.h>

int mtmprintf(int prio, const char* __restrict format, ...);

int mtmputs(int prio, const char* __restrict msg_str);

template<typename... Args>
auto mtmcout(int prio, fmt::format_string<Args...> fmt, Args&&... args) {
    uint64_t fio_allocated{};
    char* fio_ptr{};

    // this FILE IO is dynamic allocated and grows up as required
    FILE* fio{open_memstream(&fio_ptr, (size_t*)&fio_allocated)};
    fmt::print(fio, fmt, std::forward<Args>(args)...);
    fclose(fio);
    // at this point, a null byte has wrote inside of fio string

    const auto out{mtmputs(prio, fio_ptr)};
    free(fio_ptr);
    return out;
}
