
#include <cerrno>
#include <cstring>
#include <cstdlib>

#include <array>

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

            std::terminate();
        }
        static const auto mapsFormat{"/proc/%d/maps"};
        char* maps{};

        asprintf(&maps, mapsFormat, getpid());
        AFileDescriptor_setFd(g_gameEnv, jsideFile, open(maps, O_RDONLY));
        static constexpr int INVALID_FD{-1};
        if (AFileDescriptor_getFd(g_gameEnv, jsideFile) == INVALID_FD) {
            salog::printFormat(salog::LogId::Error, "Can't open the maps file, cause of %s", strerror(errno));
            free(maps);
            g_gameEnv->DeleteLocalRef(jsideFile);

            return 0;
        }

        std::array<char, 0x100> streamBuffer;
        uintptr_t mapAddr{};
        auto procMap{fdopen(AFileDescriptor_getFd(g_gameEnv, jsideFile), "r")};

        while (fgets(streamBuffer.data(), streamBuffer.size(), procMap)) {
            if (strstr(streamBuffer.data(), shared)) {
                mapAddr = (typeof(mapAddr))strtoul(streamBuffer.data(), 0, 16);
                break;
            }
        }
        free(maps);
        fclose(procMap);
        g_gameEnv->DeleteLocalRef(jsideFile);

        return mapAddr;
    }

}

