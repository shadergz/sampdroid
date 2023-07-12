#pragma once

#include <string_view>

class TextureDatabaseRuntime {
public:
    uintptr_t GetTexture(const std::string_view texName);

    uintptr_t textureLoadNew(const std::string_view dbName, const std::string_view textureName);

};

namespace saglobal {
    extern class TextureDatabaseRuntime* g_textureDatabase;
}
