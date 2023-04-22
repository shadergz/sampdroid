
#include <cerrno>
#include <cstring>
#include <cstdlib>

#include <unistd.h>
#include <malloc.h>
#include <jni.h>

#include <linux_hierarchy.h>
#include <outside.h>

extern JNIEnv* g_game_env;

uintptr_t fhsGetLibrary(const char* shared)
{
    const jobject j_file{AFileDescriptor_create(g_game_env)};
    if (j_file == nullptr) {
        const jboolean has_exec{g_game_env->ExceptionCheck()};
        if (!has_exec) 
            return 0;

        g_game_env->ExceptionDescribe();
        g_game_env->ExceptionClear();
    }
    static const char* maps_format{"/proc/%d/maps"};
    char* maps{};

    asprintf(&maps, maps_format, getpid());
    AFileDescriptor_setFd(g_game_env,
        j_file, open(maps, O_RDONLY));

    static constexpr int INVALID_FD = (int)-1;
    if (AFileDescriptor_getFd(g_game_env, j_file) == INVALID_FD) {
        mtmprintf(ANDROID_LOG_ERROR, "can't open the maps file, cause of %s\n", 
            strerror(errno));
        free(maps);
        g_game_env->DeleteLocalRef(j_file);

        return 0;
    }
    char stream_buffer[0x100];
    uintptr_t map_addr{};
    FILE* proc_map{fdopen(AFileDescriptor_getFd(g_game_env, j_file), "r")};
    
    while (fgets(stream_buffer, sizeof stream_buffer, proc_map) != NULL) {
        if (strstr(stream_buffer, shared)) {
            map_addr = (typeof(map_addr))strtoul(stream_buffer, 0, 16);
            break;
        }
    }
    free(maps);
    fclose(proc_map);
    g_game_env->DeleteLocalRef(j_file);
    
    return map_addr;
}

