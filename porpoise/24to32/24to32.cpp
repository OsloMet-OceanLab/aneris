#include "24to32.hpp"

#include <cstdint>
#include <array>

static inline int32_t _24to32(std::array<uint8_t, 3> byteArray)
{
    return (((int32_t)(byteArray[0]) << 24) |
            ((int32_t)(byteArray[1]) << 16) |
            ((int32_t)(byteArray[2]) << 8)) >> 8;
}

byte* parseNum(byte* buf, size_t len)
{
    byte* tmpbuf = (byte*)malloc(sizeof(byte)*4*len/3);
    std::array<uint8_t, 3> tmparr;

    for (size_t i = 0; i < len; i += 3)
    {
        tmparr[0] = (uint8_t) buf[i];
        tmparr[1] = (uint8_t) buf[i+1];
        tmparr[2] = (uint8_t) buf[i+2];

        int32_t tmpint = _24to32(tmparr);

        tmpbuf[i] =     (byte) (tmpint & (0xF << 12));  //  1111000000000000
        tmpbuf[i+1] =   (byte) (tmpint & (0xF << 8));   //  0000111100000000
        tmpbuf[i+2] =   (byte) (tmpint & (0xF << 4));   //  0000000011110000
        tmpbuf[i+3] =   (byte) (tmpint & (0xF));        //  0000000000001111
    }
    return tmpbuf;
}
