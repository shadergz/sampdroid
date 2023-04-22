
#include <cerrno>
#include <cstring>
#include <cstdlib>

#include <unistd.h>
#include <malloc.h>
#include <jni.h>

#include <linux_hierarchy.h>
#include <outside.h>

extern JNIEnv* g_gameEnv;

uintptr_t fhsGetLibrary(const char* shared)
{
    const jobject jFile{AFileDescriptor_create(g_gameEnv)};
    if (jFile == nullptr) {
        const jboolean hadExc{g_gameEnv->ExceptionCheck()};
        if (!hadExc) return 0;

        g_gameEnv->ExceptionDescribe();
        g_gameEnv->ExceptionClear();
    }
    static const char* mapsFormat{"/proc/%d/maps"};
    
    char* maps{};
    FILE* procMap{};
    uintptr_t mapAddr{};

    asprintf(&maps, mapsFormat, getpid());
    AFileDescriptor_setFd(g_gameEnv,
        jFile, open(maps, O_RDONLY));

    static constexpr int INVALID_FD = (int)-1;
    if (AFileDescriptor_getFd(g_gameEnv, jFile) == INVALID_FD) {
        mtmprintf(ANDROID_LOG_ERROR, "Can't open the maps file, cause of %s\n", 
            strerror(errno));
        free(maps);
        fclose(procMap);
        g_gameEnv->DeleteLocalRef(jFile);
    }
    char streamBuffer[0x100];
    procMap = fdopen(AFileDescriptor_getFd(g_gameEnv, jFile), "r");
    
    while (fgets(streamBuffer, sizeof streamBuffer, procMap) != NULL) {
        if (strstr(streamBuffer, shared)) {
            mapAddr = (typeof(mapAddr))strtoul(streamBuffer, 0, 16);
            break;
        }
    }
    free(maps);
    fclose(procMap);
    g_gameEnv->DeleteLocalRef(jFile);
    
    return mapAddr;
}

