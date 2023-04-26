#pragma once

#include <cstdint>
#include <fcntl.h>

#include <android/file_descriptor_jni.h>

uintptr_t fhsGetLibrary(const char* shared);

