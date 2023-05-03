#pragma once

#include <cstdint>

namespace Mt4Global {
extern void (*g_NVThreadSpawnProc)(uintptr_t x0);
}

namespace Mt4Mimic {
void NVThreadSpawnProc(uintptr_t x0);
}

