#pragma once

#include <android/log.h>
#include <cstdio>

#include <utility>
#include <string_view>

#ifndef NDEBUG
#include <fmt/std.h>
#endif

enum LogId {
    Info = ANDROID_LOG_INFO,
    Debug = ANDROID_LOG_DEBUG,
    Error = ANDROID_LOG_ERROR,
};

static int userDisplay(const LogId mode, const char* format, va_list va);
#define USER_DSP_FUNC(mode, fmt)\
    va_list va;\
    va_start(va, fmt);\
    const auto print = userDisplay(mode, fmt, va);\
    va_end(va);\
    return print

int useriDsp(const char* format, ...) {
    USER_DSP_FUNC(Info, format);
}
int userdDsp(const char* format, ...) {
    USER_DSP_FUNC(Debug, format);
}
int usereDsp(const char* format, ...) {
    USER_DSP_FUNC(Error, format);
}

#ifndef NDEBUG
template<typename... Args>
auto coutFmt(const LogId prio, fmt::format_string<Args...> fmt, Args&&... args)
{
    uint64_t fioAllocated{};
    char* fioPtr{};

    // this FILE IO is dynamic allocated and grows up as required
    auto fio = open_memstream(&fioPtr, reinterpret_cast<size_t*>(&fioAllocated))};
    fmt::print(fio, fmt, std::forward<Args>(args)...);
    fclose(fio);
    // at this point, a null byte has wrote inside of fio string

    const auto out = print(prio, fioPtr)};
    free(fioPtr);
    return out;
}
#else
template<typename... Args>
auto coutFmt([[maybe_unused]] LogId prio, [[maybe_unused]] const std::string_view formatUnused,
    [[maybe_unused]] Args&&... args)
{
    return 0;
}
#endif

void assertAbort(const char* cond, const char* fileName,
    int line, const char* format, ...);

#define SALOG_ASSERT(cond, fmt, ...)\
    if (!(cond))\
        assertAbort(#cond, __FILE__, __LINE__, fmt, #__VA_ARGS__)
