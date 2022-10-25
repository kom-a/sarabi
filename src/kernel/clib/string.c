#include "string.h"

uint32_t StringLength(const char* str)
{
    int i = 0;
    while (str[i] != '\0')
        ++i;
    return i;
}

void StringAppend(char* str, char c)
{
    int len = StringLength(str);
    str[len] = c;
    str[len + 1] = '\0';
}

void StringCopy(char* dest, const char* src)
{
    while(*src != '\0')
    {
        *dest++ = *src++;
    }

    *dest = '\0';
}