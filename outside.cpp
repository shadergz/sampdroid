
#include <outside.h>

namespace Mt4Global {
const char* g_logTag = "MTM";
}

namespace Mt4Log {
int printflike(int prio, const char* __restrict format, ...)
{
    va_list var;
    va_start(var, format);

    const auto fret{__android_log_vprint(prio, 
        Mt4Global::g_logTag, format, var)};
    va_end(var);

    return fret;
}

int putslike(int prio, const char* __restrict msg_str)
{
    const auto wStr{__android_log_write(prio, Mt4Global::g_logTag, msg_str)};
    return wStr;
}

void assertAbort(const char* cond, const char* fileName, 
    int line, const char* __restrict format, ...)
{
    thread_local char assert_msg[0x5f];
    if (!cond)
        return;
    
    va_list va;
    va_start(va, format);
    vsnprintf(assert_msg, sizeof assert_msg, format, va);
    va_end(va);

    __android_log_assert(cond, Mt4Global::g_logTag, "%s:%d -> %s", 
        fileName, line, assert_msg);
}

}
