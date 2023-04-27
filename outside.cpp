
#include <outside.h>

const char* g_logtag = "MT-Mobile";

int mtmprintf(int prio, const char* __restrict format, ...)
{
    va_list var;
    va_start(var, format);

    const auto fret{__android_log_vprint(prio, g_logtag, format, var)};
    va_end(var);

    return fret;
}

int mtmputs(int prio, const char* __restrict msg_str)
{
    const auto wStr{__android_log_write(prio, g_logtag, msg_str)};
    return wStr;
}