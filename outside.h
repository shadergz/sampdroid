#pragma once

#include <android/log.h>

int mtmprintf(int prio, const char* __restrict format, ...);

int mtmputs(int prio, const char* __restrict msgStr);