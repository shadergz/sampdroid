
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
    static const char* mapsFormat{"/proc/%d/maps"};
    char* maps{};
    FILE* procMap{};
    
    jobject jFile{AFileDescriptor_create(g_gameEnv)};
    uintptr_t mapAddr{};

    if (jFile == nullptr) {
        const jboolean hprob{g_gameEnv->ExceptionCheck()};
        if (!hprob) goto finishLoad;

        g_gameEnv->ExceptionDescribe();
        g_gameEnv->ExceptionClear();
    }

    asprintf(&maps, mapsFormat, getpid());
    if (!maps) 
        // Also nullptr when converting to void*
        goto finishLoad;
    
    AFileDescriptor_setFd(g_gameEnv,
        jFile, open(maps, O_RDONLY));

    if (AFileDescriptor_getFd(g_gameEnv, jFile) < 2) {
        mtmprintf(ANDROID_LOG_ERROR, "Can't open the maps file, cause of %s\n", 
            strerror(errno));
        goto finishLoad;
    }
    char streamBuffer[0x100];
    procMap = fdopen(AFileDescriptor_getFd(g_gameEnv, jFile), "r");
    
    while (fgets(streamBuffer, 
        sizeof streamBuffer, procMap) != NULL) {
        if (strstr(streamBuffer, shared)) {
            mapAddr = (typeof(mapAddr))strtoul(
                streamBuffer,
                0, 
                16);
            break;
        }
    }

finishLoad:
    free(maps);
    fclose(procMap);

    g_gameEnv->DeleteLocalRef(jFile);
    return mapAddr;
}

