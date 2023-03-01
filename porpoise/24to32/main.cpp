#include <iostream>
#include <cstdlib>
#include "24to32.hpp"

int main()
{
    byte *buffer = (byte*)malloc(sizeof(byte)*1017);
    File* fptr = fopen("../packet", "rb");
    if(!fptr)
    {
        fprintf(stderr, "Couldn't open file");
        exit(1);
    }
    fread(buffer, 1017, 1, fptr);

    printf("%02X %02X", (uint8_t)buffer[0], (uint8_t)buffer[0]);

    return 0;
}
