
#include <log_client.h>

namespace saglobal {
    const char* g_logTag = "clientsa";
}

namespace salog {
    int printFormat(int prio, const char* __restrict format, ...)
    {
        va_list var;
        va_start(var, format);
    
        const auto fRet{__android_log_vprint(prio, saglobal::g_logTag, format, var)};
        va_end(var);
    
        return fRet;
    }
    
    int print(int prio, const char* __restrict msgStr)
    {
      const auto wrResult{__android_log_write(prio, saglobal::g_logTag, msgStr)};
      return wrResult;
    }
    
    void assertAbort(const char* cond, const char* fileName, 
        int line, const char* __restrict format, ...)
    {
        thread_local char assertBuffer[0x5f];
        if (!cond)
            return;
        
        va_list va;
        va_start(va, format);
        vsnprintf(assertBuffer, sizeof assertBuffer, format, va);
        va_end(va);
    
        __android_log_assert(cond, saglobal::g_logTag, "%s:%d -> %s", fileName, line, assertBuffer);
    }
    
}
