#include <array>

#include <log_client.h>

namespace saglobal {
    const std::string_view g_logTag = "saclient";
}

namespace salog {
    using namespace saglobal;

#ifndef NDEBUG
    int printFormat(int prio, const std::string_view format, ...)
    {
        va_list var;
        va_start(var, format);
    
        const auto droidRet{__android_log_vprint(prio, g_logTag.data(), format.data(), var)};
        va_end(var);
    
        return droidRet;
    }
    
    int print(int prio, const std::string_view msgStr)
    {
        const auto androidResult{__android_log_write(prio, g_logTag.data(), msgStr.data())};
        return androidResult;
    }
    
#else
    int printFormat([[maybe_unused]] int prio, [[maybe_unused]] const std::string_view format, ...)
    {
        return 0;
    }
    int print([[maybe_unused]] int prio, [[maybe_unused]] const std::string_view msgStr)
    {
        return 0;
    }
#endif

    void assertAbort(const std::string_view cond, const std::string_view fileName,
        int line, const std::string_view format, ...)
    {
        thread_local std::array<char, 0x5f> assertBuffer;
        if (!cond.data())
            return;

        va_list va;
        va_start(va, format);
        vsnprintf(assertBuffer.data(), assertBuffer.size(), format.data(), va);
        va_end(va);

        __android_log_assert(nullptr, g_logTag.data(), "ASSERTATION: By %s in %s:%d -> %s",
            cond.data(), fileName.data(), line, assertBuffer.data());
        std::terminate();
    }
    
}
