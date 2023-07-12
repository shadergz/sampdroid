#pragma once

#include <cstdint>
#include <fcntl.h>

#include <android/file_descriptor_jni.h>
#include <string_view>

namespace safs {
    uintptr_t getLibrary(const std::string_view shared);

}
