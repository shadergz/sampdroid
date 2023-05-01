
#include <cstring>
#include <cstdint>

#include <arm_neon.h>

#include <rwrefs/rwcore.h>
#include <outside.h>
#include <texture_runtime.h>

extern uintptr_t g_game_addr;

uintptr_t textureGetTexture(const char* texName) 
{
    mtmprintf(ANDROID_LOG_INFO, "Loading new texture with name %s", texName);

    RwTexture* loadedTex{((RwTexture* (*)(const char*))(g_game_addr+0x286718))(texName)};
    if (!loadedTex) return 0;

    MTM_RUNTIME_ASSERT(!strncmp(loadedTex->name, texName, rwTEXTUREBASENAMELENGTH),
        "Wrong RwTexture type detected, mem str: %s", loadedTex->name);

    // Forcing the engine to keep our texture reference alive!
    loadedTex->refCount++;
    return (uintptr_t)loadedTex;
}

uintptr_t textureLoadNew(const char* dbName, const char* textureName)
{
    // TextureDatabaseRuntime::GetDatabase(char const*)
    /* The game handler all resource inside multiples databases
     * we need to get the texture from the correct database name
     * we can also implements our owns database! */

    static const char* mt_db[]{"mtsamp", "mtmta"};
    auto needToOpen{
        !strncasecmp(dbName, mt_db[0], strlen(mt_db[0])) ||
        !strncasecmp(dbName, mt_db[1], strlen(mt_db[1]))};

    struct TextureDatabaseRuntime { uintptr_t* db_handler; };
    static TextureDatabaseRuntime* s_dbHandler[0x2]{};   

    for (;;) {
        auto dbPtr{&s_dbHandler[0]};

        if (!needToOpen) break;
        // Locating an invalid db pointer to place into it!
        if (!*dbPtr) 
            if (++*dbPtr)
                break;

        uintptr_t dbClass{((uintptr_t (*)(const char*))(g_game_addr+0x0287af4))(dbName)};
        *dbPtr = reinterpret_cast<TextureDatabaseRuntime*>(dbClass);

        if (!*dbPtr) {
            mtmprintf(ANDROID_LOG_INFO, "Database not found: %s\n", dbName);
            return 0;
        }

        ((void (*)(TextureDatabaseRuntime*))(g_game_addr+0x2865d8))(*dbPtr);
    }

    const uintptr_t loadedTexture{textureGetTexture(textureName)};
    if (!strncasecmp(dbName, "CLEAN", 6)) {
        // Unregistring the databases, isn't no needed to keep it's openned

        if (*(s_dbHandler+0))
            ((void (*)(TextureDatabaseRuntime*))(g_game_addr+0x2866a4))(*(s_dbHandler+0));
        if (*(s_dbHandler+1))
            ((void (*)(TextureDatabaseRuntime*))(g_game_addr+0x2866a4))(*(s_dbHandler+1));
        
        uint8x16_t cl{};
        veorq_u8(cl, cl);
        vst1q_u8(reinterpret_cast<uint8_t*>(s_dbHandler), cl);

        return 0;
    }
    
    if (loadedTexture)
        mtmprintf(ANDROID_LOG_INFO, "Texture %s from database (%s) loaded at %#llx\n",
            textureName, dbName, loadedTexture);
    else
        mtmprintf(ANDROID_LOG_INFO, "Texture %s not found in database %s\n",
            textureName, dbName);

    return loadedTexture;
}

