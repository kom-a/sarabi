#pragma once

#include "../inttypes.h"

uint32_t StringLength(const char* str);
void StringAppend(char* str, char c);
void StringCopy(char* dest, const char* src);