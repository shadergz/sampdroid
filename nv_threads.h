#pragma once

#include <cstdint>

extern void (*NVThreadSpawnProc)(uintptr_t x0);
void NVThreadSpawnProc_HOOK(uintptr_t x0);

