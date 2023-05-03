
#include <cstring>
#include <cstdint>

#include <arm_neon.h>

#include <rwrefs/rwcore.h>
#include <outside.h>
#include <texture_runtime.h>

namespace Mt4Global {
extern uintptr_t g_gameAddr;
}

namespace Mt4Global {
class TextureDatabaseRuntime* g_textureDatabase;
}

uintptr_t TextureDatabaseRuntime::GetTexture(const char* texName) 
{
    Mt4Log::printflike(ANDROID_LOG_INFO, "Loading new texture with name %s", texName);

    RwTexture* loadedTex{((RwTexture* (*)(const char*))(Mt4Global::g_gameAddr+0x286718))(texName)};
    if (!loadedTex) 
        return 0;

    MT4LOG_RUNTIME_ASSERT(!strncmp(loadedTex->name, texName, rwTEXTUREBASENAMELENGTH),
        "Wrong RwTexture type detected, mem str: %s", loadedTex->name);

    // Forcing the engine to keep our texture reference alive!
    loadedTex->refCount++;
    return (uintptr_t)loadedTex;
}

uintptr_t TextureDatabaseRuntime::textureLoadNew(const char* dbName, const char* textureName)
{
    // TextureDatabaseRuntime::GetDatabase(char const*)
    /* The game handler all resource inside multiples databases
     * we need to get the texture from the correct database name
     * we can also implements our owns database! */

    static const char* mtDB[]{
        "mt4m", 
        "playerside", 
        "serverside"};
    
    bool needToOpen{false};
    for (auto mtPrivateDb : mtDB) {
        if (!strncasecmp(dbName, mtPrivateDb, strlen(mtPrivateDb)))
            needToOpen = true;
    }
    
    struct NativeTDRHandler { uintptr_t* dbHandler; };
    static NativeTDRHandler* dbHandler[std::size(mtDB)]{};

    for (;;) {
        auto dbPtr{&dbHandler[0]};

        if (!needToOpen) break;
        // Locating an invalid db pointer to place into it!
        while (!*dbPtr) dbPtr++;
        if (dbPtr >= &dbPtr[std::size(dbHandler)]) break;

        uintptr_t dbClass{((uintptr_t (*)(const char*))(Mt4Global::g_gameAddr+0x0287af4))(dbName)};
        *dbPtr = reinterpret_cast<NativeTDRHandler*>(dbClass);

        if (!*dbPtr) {
            Mt4Log::printflike(ANDROID_LOG_INFO, "Database not found: %s\n", dbName);
            return 0;
        }

        ((void (*)(NativeTDRHandler*))(Mt4Global::g_gameAddr+0x2865d8))(*dbPtr);
    }

    const uintptr_t loadedTexture{GetTexture(textureName)};
    if (!strncasecmp(dbName, "CLEAN", 6)) {
        // Unregistring the databases, isn't no needed to keep it's openned

        if (*(dbHandler+0))
            ((void (*)(NativeTDRHandler*))(Mt4Global::g_gameAddr+0x2866a4))(*(dbHandler+0));
        if (*(dbHandler+1))
            ((void (*)(NativeTDRHandler*))(Mt4Global::g_gameAddr+0x2866a4))(*(dbHandler+1));
        
        uint8x16_t cl{};
        veorq_u8(cl, cl);
        vst1q_u8(reinterpret_cast<uint8_t*>(dbHandler), cl);

        return 0;
    }
    
    if (loadedTexture)
        Mt4Log::printflike(ANDROID_LOG_INFO, "Texture %s from database (%s) loaded at %#llx\n",
            textureName, dbName, loadedTexture);
    else
        Mt4Log::printflike(ANDROID_LOG_INFO, "Texture %s not found in database %s\n",
            textureName, dbName);

    return loadedTexture;
}


