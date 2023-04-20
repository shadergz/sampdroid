#pragma once

#include <stdint.h>
#include <fcntl.h>

#include <android/file_descriptor_jni.h>

uintptr_t fhsGetLibrary(const char* shared);

