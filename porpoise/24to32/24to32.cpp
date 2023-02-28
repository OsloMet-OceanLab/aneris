#include "24to32.hpp"

#include <cstdint>

static inline int32_t _24to32(std::array<uint8_t, 3> byteArray)
{
    return (((int32_t)(byteArray[0]) << 24) |
            ((int32_t)(byteArray[1]) << 16) |
            ((int32_t)(byteArray[2]) << 8)) >> 8; 
}

char* parseNum(char* buf, size_t len)
{
    char* tmpbuf = (char*)malloc(sizeof(char)*len*4/3);
    for (size_t i = 0; i < len; i += 3)
        memcpy(tmpbuf, _24to32(buf), 4);
}
