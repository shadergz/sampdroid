#pragma once

#include <cstdint>
#include <fcntl.h>

#include <android/file_descriptor_jni.h>

namespace Mt4Fs {
uintptr_t getLibrary(const char* shared);

}
