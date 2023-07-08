#pragma once

#include <cstdint>

class TextureDatabaseRuntime {
public:
    uintptr_t GetTexture(const char* texName);

    uintptr_t textureLoadNew(const char* dbName, const char* textureName);

};

namespace saglobal {
    extern class TextureDatabaseRuntime* g_textureDatabase;
}
