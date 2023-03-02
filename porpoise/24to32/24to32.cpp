#include "24to32.hpp"

#include <array>
#include <cstdint>
#include <vector>

namespace _24bit_converter
{

static inline int16_t _24to16(const byte byteArray[])
{
    return (((uint16_t) (byteArray[1] << 8)) | (uint8_t) byteArray[2]);
}

byte* numto16(const byte *buf, size_t len, bool invertEndianness)
{
    std::vector<int16_t> tmp_arr;

    byte* newbuf = (byte*)malloc(sizeof(byte)*2*len/3);

    for (size_t i = 0; i < len; i += 3)
        tmp_arr.push_back(tmparr2.push_back(_24to16(buf+i)));

    for (size_t i = 0; i < 2*len/3; i += 2)
    {
        *(newbuf + i) =       0xFF & (tmp_arr[i/2] >> (invertEndianness ? 0 : 8));
        *(newbuf + i + 1) =   0xFF & (tmp_arr[i/2] >> (invertEndianness ? 8 : 0));
    }
    return newbuf;
}

static inline int32_t _24to32(std::array<uint8_t, 3> byteArray)
{
    return (((int32_t)(byteArray[0]) << 24) |
            ((int32_t)(byteArray[1]) << 16) |
            ((int32_t)(byteArray[2]) << 8)) >> 8;
}

byte* numto32(const byte *buf, size_t len, bool invertEndianness)
{
    std::array<uint8_t, 3> tmparr;
    std::vector<int32_t> tmparr2;

    byte* newbuf = (byte*)malloc(sizeof(byte)*4*len/3);

    for (size_t i = 0; i < len; i += 3)
    {
        tmparr[0] = (uint8_t) buf[i];
        tmparr[1] = (uint8_t) buf[i+1];
        tmparr[2] = (uint8_t) buf[i+2];

        tmparr2.push_back(_24to32(tmparr));
    }

    for (size_t i = 0; i < 4*len/3; i += 4)
    {
        *(newbuf + i) =       0xFF & (tmparr2[i/4] >> (invertEndianness ? 0 : 24));
        *(newbuf + i + 1) =   0xFF & (tmparr2[i/4] >> (invertEndianness ? 8 : 16));
        *(newbuf + i + 2) =   0xFF & (tmparr2[i/4] >> (invertEndianness ? 16 : 8));
        *(newbuf + i + 3) =   0xFF & (tmparr2[i/4] >> (invertEndianness ? 24 : 0));
    }
    return newbuf;
}

} // end namespace _24bit_converter
