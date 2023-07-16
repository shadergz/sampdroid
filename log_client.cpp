#include <array>

#include <log_client.h>

namespace saglobal {
    const char* g_logTag = "saclient";
}

namespace salog {
    using namespace saglobal;

#ifndef NDEBUG
    int printFormat(int prio, const char* format, ...)
    {
        va_list var;
        va_start(var, format);
    
        const auto droidRet{__android_log_vprint(prio, g_logTag, format, var)};
        va_end(var);
    
        return droidRet;
    }
    
    int print(int prio, const char* msgStr)
    {
        const auto androidResult{__android_log_write(prio, g_logTag, msgStr)};
        return androidResult;
    }
    
#else
    int printFormat([[maybe_unused]] int prio, [[maybe_unused]] const char* format, ...)
    {
        return 0;
    }
    int print([[maybe_unused]] int prio, [[maybe_unused]] const char* msgStr)
    {
        return 0;
    }
#endif

    void assertAbort(const char* cond, const char* fileName,
        int line, const char* format, ...)
    {
        thread_local std::array<char, 0x5f> assertBuffer;
        if (!cond) return;

        va_list va;
        va_start(va, format);
        vsnprintf(assertBuffer.data(), assertBuffer.size(), format, va);
        va_end(va);

        __android_log_assert(nullptr, g_logTag, "ASSERTATION: By %s in %s:%d -> %s", cond, fileName,
            line, assertBuffer.data());
        std::terminate();
    }
    
}
