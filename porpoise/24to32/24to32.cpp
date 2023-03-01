#include "24to32.hpp"

#include <cstdint>
#include <array>

static inline int32_t _24to32(std::array<uint8_t, 3> byteArray)
{
    return (((int32_t)(byteArray[0]) << 24) |
            ((int32_t)(byteArray[1]) << 16) |
            ((int32_t)(byteArray[2]) << 8)) >> 8;
}

byte* parseNum(const byte* buf, size_t len)
{
    std::array<uint8_t, 3> tmparr;
    byte* newbuf = (byte*)malloc(sizeof(byte)*4*len/3);

    for (size_t i = 0; i < len; i += 3)
    {
        tmparr[0] = (uint8_t) buf[i];
        tmparr[1] = (uint8_t) buf[i+1];
        tmparr[2] = (uint8_t) buf[i+2];

        int32_t tmpint = _24to32(tmparr);

        newbuf[i] =     (byte) (tmpint & (0xFF << 24));
        newbuf[i+1] =   (byte) (tmpint & (0xFF << 16));
        newbuf[i+2] =   (byte) (tmpint & (0xFF << 8));
        newbuf[i+3] =   (byte) (tmpint & (0xFF << 0));
    }
    return newbuf;
}
