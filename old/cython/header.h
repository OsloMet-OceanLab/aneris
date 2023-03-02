#ifndef HEADER_H
#define HEADER_H

#include <iostream>
#include <cstdint>

int32_t square(int32_t x)
{
    return x * x;
}

char* printTxt(char* s)
{
    printf("%s", s);
    return s;
}

#endif
