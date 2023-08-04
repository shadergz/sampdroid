
#include <iterator>
#include <cstring>

#include <arm_neon.h>

#include <render/rwcore.h>
#include <log_client.h>
#include <texture_runtime.h>

namespace saglobal {
    extern uintptr_t g_gameAddr;
    class TextureDatabaseRuntime* g_textureDatabase;
}

uintptr_t TextureDatabaseRuntime::GetTexture(const char* textureName)
{
    salog::printFormat(salog::Info, "DB: loading new texture with name %s in memory", textureName);
    auto loadedTex{((RwTexture* (*)(const char*))(saglobal::g_gameAddr + 0x286718))(textureName)};
    if (!loadedTex)
        return 0;

    SALOG_ASSERT(!std::strncmp(loadedTex->name, textureName, rwTEXTUREBASENAMELENGTH),
        "Wrong RwTexture type detected, memory str ref: %s", loadedTex->name);

    // Forcing the engine to keep our texture reference alive!
    loadedTex->refCount++;
    return (uintptr_t)loadedTex;
}

uintptr_t TextureDatabaseRuntime::textureLoadNew(const char* dbName, const char* textureName)
{
    // TextureDatabaseRuntime::GetDatabase(char const*), The game handler all resource inside multiples
    // databases, we need to get the texture from the correct database name, we can also
    // implements our owns databases
    static const char* mtDB[]{
        "client",
        "playerside", 
        "serverside"
    };
    
    bool needToOpen{false};
    for (auto mtPrivateDb : mtDB) {
        if (!std::strncmp(mtPrivateDb, dbName, strlen(dbName)))
            needToOpen = true;
    }
    
    struct NativeTDRHandler {
        uintptr_t* m_dbHandler;
    };
    static NativeTDRHandler* dbHandler[std::size(mtDB)]{};

    for (;;) {
        auto dbPtr{&dbHandler[0]};
        if (!needToOpen)
            break;
        // Locating an invalid database pointer to place into it!
        while (!*dbPtr && dbPtr < &dbHandler[std::size(dbHandler)])
            dbPtr++;
        if (dbPtr >= &dbPtr[std::size(dbHandler)])
          break;

        auto dbClass{((uintptr_t (*)(const char*))(saglobal::g_gameAddr + 0x0287af4))(dbName)};
        *dbPtr = reinterpret_cast<NativeTDRHandler*>(dbClass);
        if (!*dbPtr) {
            salog::printFormat(salog::Error, "DB: database not found: %s", dbName);
            std::terminate();
        }

        ((void (*)(NativeTDRHandler*))(saglobal::g_gameAddr + 0x2865d8))(*dbPtr);
    }

    const auto loadedTexture{GetTexture(textureName)};
    
    static const char cleanTrigger[]{"clean"};

    if (!std::strncmp(dbName, cleanTrigger, sizeof(cleanTrigger))) {
        // Unregistring the databases, isn't no needed to keep it's openned
        if (*(dbHandler + 0x0))
            ((void (*)(NativeTDRHandler*))(saglobal::g_gameAddr + 0x2866a4))(*(dbHandler + 0x0));
        if (*(dbHandler + 0x1))
            ((void (*)(NativeTDRHandler*))(saglobal::g_gameAddr + 0x2866a4))(*(dbHandler + 0x1));
        if (*(dbHandler + 0x2))
            ((void (*)(NativeTDRHandler*))(saglobal::g_gameAddr + 0x2866a4))(*(dbHandler + 0x2));
        
        uint8x16_t cl{};
        veorq_u8(cl, cl);
        vst1q_u8(reinterpret_cast<uint8_t*>(dbHandler), cl);

        return 0;
    }
    
    if (loadedTexture)
        salog::printFormat(salog::Info, "DB: texture %s from database (%s) loaded at %#llx",
            textureName, dbName, loadedTexture);
    else
        salog::printFormat(salog::Error, "DB: texture %s not found in database %s", textureName, dbName);

    return loadedTexture;
}


