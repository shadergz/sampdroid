
#include <cerrno>
#include <cstring>
#include <cstdlib>

#include <unistd.h>
#include <malloc.h>
#include <jni.h>

#include <linux_hierarchy.h>
#include <log_client.h>

namespace saglobal {
    extern JNIEnv* g_gameEnv;
}

namespace safs {
    using namespace saglobal;

    uintptr_t getLibrary(const char* shared)
    {
        const jobject jsideFile{AFileDescriptor_create(g_gameEnv)};
        if (jsideFile == nullptr) {
            const jboolean hasExec{g_gameEnv->ExceptionCheck()};
            if (!hasExec)
                return 0;

            g_gameEnv->ExceptionDescribe();
            g_gameEnv->ExceptionClear();
        }
        static const char* mapsFormat{"/proc/%d/maps"};
        char* maps{};

        asprintf(&maps, mapsFormat, getpid());
        AFileDescriptor_setFd(g_gameEnv,
            jsideFile, open(maps, O_RDONLY));

        static constexpr int INVALID_FD{-1};
        if (AFileDescriptor_getFd(g_gameEnv, jsideFile) == INVALID_FD) {
            salog::printFormat(ANDROID_LOG_ERROR, "Can't open the maps file, cause of %s\n", strerror(errno));
            free(maps);
            g_gameEnv->DeleteLocalRef(jsideFile);

            return 0;
        }
        char streamBuffer[0x100];
        uintptr_t mapAddr{};
        auto procMap{fdopen(AFileDescriptor_getFd(g_gameEnv, jsideFile), "r")};

        while (fgets(streamBuffer, sizeof streamBuffer, procMap) != NULL) {
            if (strstr(streamBuffer, shared)) {
                mapAddr = (typeof(mapAddr))strtoul(streamBuffer, 0, 16);
                break;
            }
        }
        free(maps);
        fclose(procMap);
        g_gameEnv->DeleteLocalRef(jsideFile);

        return mapAddr;
    }

}

