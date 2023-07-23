
#include <cstdint>

namespace samimic {
    long CGame_InitializeRenderWare();
}

namespace saglobal {
    extern long (*g_CGame_InitializeRenderWare)();
}
