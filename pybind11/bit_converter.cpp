#include "bit_converter.hpp"

#include <array>
#include <cstdint>
#include <vector>

namespace bit_converter
{

static inline int16_t _24to16(const byte byteArray[])
{
    return (((uint16_t) (byteArray[1] << 8)) | (uint8_t) byteArray[2]);
}

py::bytes numto16(const byte *buf, size_t len, bool invertEndianness)
{
    if (!buf || !len) return nullptr;

    int16_t *tmp_arr = (int16_t*)malloc(sizeof(int16_t)*len/3);
    byte* newbuf = (byte*)malloc(sizeof(byte)*2*len/3);
    //std::vector<int16_t> tmp_arr;

    for (size_t i = 0; i < len; i += 3)
        //tmp_arr.push_back(_24to16(buf+i));
        *(tmp_arr + (i/3)) = _24to16(buf+i);

    for (size_t i = 0; i < 2*len/3; i += 2)
    {
        *(newbuf + i) =       0xFF & (tmp_arr[i/2] >> (invertEndianness ? 0 : 8));
        *(newbuf + i + 1) =   0xFF & (tmp_arr[i/2] >> (invertEndianness ? 8 : 0));
    }

    free(tmp_arr);

    return py::bytes(newbuf);
}

static inline int32_t _24to32(std::array<uint8_t, 3> byteArray)
{
    return (((int32_t)(byteArray[0]) << 24) |
            ((int32_t)(byteArray[1]) << 16) |
            ((int32_t)(byteArray[2]) << 8)) >> 8;
}

py::bytes numto32(const byte *buf, size_t len, bool invertEndianness)
{
    if (!buf || !len) return nullptr;

    int32_t *tmp_int_arr = (int32_t*)malloc(sizeof(int32_t)*len/3);
    std::array<uint8_t, 3> tmparr;
    //std::vector<int32_t> tmparr2;
    byte* newbuf = (byte*)malloc(sizeof(byte)*4*len/3);

    for (size_t i = 0; i < len; i += 3)
    {
        tmparr[0] = (uint8_t) buf[i];
        tmparr[1] = (uint8_t) buf[i+1];
        tmparr[2] = (uint8_t) buf[i+2];

        //tmparr2.push_back(_24to32(tmparr));
        *(tmp_int_arr + (i/3)) = _24to32(tmparr);
    }

    for (size_t i = 0; i < 4*len/3; i += 4)
    {
        *(newbuf + i) =       0xFF & (tmp_int_arr[i/4] >> (invertEndianness ? 0 : 24));
        *(newbuf + i + 1) =   0xFF & (tmp_int_arr[i/4] >> (invertEndianness ? 8 : 16));
        *(newbuf + i + 2) =   0xFF & (tmp_int_arr[i/4] >> (invertEndianness ? 16 : 8));
        *(newbuf + i + 3) =   0xFF & (tmp_int_arr[i/4] >> (invertEndianness ? 24 : 0));
    }

    free(tmp_int_arr);

    return py::bytes(newbuf);
}

py::bytes invertEndianness24(const byte *buf, size_t len)
{
    byte *newbuf = (byte*)malloc(sizeof(byte)*len);

    for (int i = 0; i < len; i += 3)
    {
        *(newbuf + i) = *(buf + i + 2);
        *(newbuf + i + 1) = *(buf + i + 1);
        *(newbuf + i + 2) = *(buf + i);
    }
    return newbuf;
}

} // end namespace bit_converter
