#pragma once

#include <cstdint>

namespace saglobal {
    extern void (*g_NVThreadSpawnProc)(uintptr_t x0);
}

namespace samimic {
    void NVThreadSpawnProc(uintptr_t x0);
}

