#include <array>

#include <cstdio>
#include <cstring>
#include <ctime>
#include <unistd.h>

#include <log_client.h>

namespace saglobal {
    extern uintptr_t g_gameAddr;
}

static const char* logcatTag = "saclient";

// This wrapper class is used to ensure that the file will be fclose()'d at the end of this library's lifetime
class LogFile {
public:
    ~LogFile() {
        if (m_logFile)
            fclose(m_logFile);
    }
    void operator=(std::FILE* filePtr) {
        m_logFile = filePtr;
    }

    auto operator*() {
        return m_logFile;
    }

private:
    std::FILE* m_logFile;

};

namespace salog {
    static LogFile logFile{};

    using namespace saglobal;

    [[gnu::always_inline]] inline void checkLogFile() {
        char logFilePath[0x4f];

        std::snprintf(logFilePath, std::size(logFilePath),
            "%slog_client.txt", reinterpret_cast<const char*>(g_gameAddr + 0x8b46a8));
        
        logFile = fopen(logFilePath, "a");
        char openedAt[0x2f];

        if (!*logFile) {
            // It isn't a critical error and occurs somewhat frequently at plugin startup
#ifndef NDEBUG
            __android_log_print(ANDROID_LOG_ERROR, logcatTag, 
                "Can't open the log file in (...missing...)%s", logFilePath);
#endif
            return;
        }

        auto ts{time(nullptr)};
        const auto timeData{localtime(&ts)};

        std::strftime(openedAt, std::size(openedAt), "%T", timeData);
        SALOG_ASSERT(access(logFilePath, W_OK) == 0, "Sa log file in GTA SA external data dir couldn't be found");
        
        __android_log_print(ANDROID_LOG_INFO, logcatTag,
            "Log file opened in %s", logFilePath);
        
        std::fprintf(*logFile, "Start time of writing [%s], file pointer: %p from %s\n", 
            openedAt, *logFile, logFilePath);
        fflush(*logFile);
        
    }

    int printFormat(LogId prio, const char* format, ...)
    {
#if NDEBUG
        if (prio == LogId::Debug)
            return 0;
#endif
        va_list var, cp;
        va_start(var, format);
        va_copy(cp, var);

        [[unlikely]] if (!*logFile && g_gameAddr)
            checkLogFile();
    
        const auto droidRet{__android_log_vprint(
            static_cast<android_LogPriority>(prio), logcatTag, format, var)};
        
        if (*logFile) {
            std::vfprintf(*logFile, format, cp);
            putc('\n', *logFile);
            fflush(*logFile);
        }
        
        va_end(var);
        va_end(cp);
    
        return droidRet;
    }
    
    int print(LogId prio, const char* msgStr)
    {
#if NDEBUG
        if (prio == LogId::Debug)
            return 0;
#endif

        [[unlikely]] if (!*logFile && g_gameAddr)
            checkLogFile();

        const auto androidResult{__android_log_write(
            static_cast<android_LogPriority>(prio), logcatTag, msgStr)};
        
        if (*logFile) {

            std::fputs(msgStr, *logFile);
            putc('\n', *logFile);
            fflush(*logFile);

        }
        return androidResult;
    }

    void assertAbort(const char* cond, const char* fileName,
        int line, const char* format, ...)
    {
        thread_local std::array<char, 0x5f> assertBuffer;
        if (!cond) 
            return;

        va_list va;
        va_start(va, format);
        vsnprintf(assertBuffer.data(), assertBuffer.size(), format, va);
        va_end(va);

        __android_log_assert(nullptr, logcatTag, "ASSERTION: by %s in %s:%d -> %s", cond, fileName,
            line, assertBuffer.data());
        
        std::terminate();
    }
    
}
