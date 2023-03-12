#include "bit_converter.hpp"

#include <array>
#include <cstdint>
#include <string>

namespace bit_converter
{

static inline int16_t _24to16(const char byteArray[])
{
    return (((uint16_t) (byteArray[1] << 8)) | (uint8_t) byteArray[2]);
}

#ifdef CPYTHON
py::bytes
#else
char *
#endif
bytes_to_16(const char *buf, size_t len, bool invertEndianness)
{
    if (!buf || !len) return nullptr;

    int16_t *tmp_arr = new int16_t[len/3];
    std::string newbuf;
	
    for (size_t i = 0; i < len; i += 3)
        *(tmp_arr + (i/3)) = _24to16(buf+i);

    for (size_t i = 0; i < 2*len/3; i += 2)
    {
        newbuf += 0xFF & (tmp_arr[i/2] >> (invertEndianness ? 0 : 8));
        newbuf += 0xFF & (tmp_arr[i/2] >> (invertEndianness ? 8 : 0));
    }

    delete[] tmp_arr;

#ifdef CPYTHON
    return py::bytes(newbuf);
#else
    return newbuf.c_str();
#endif
}

static inline int32_t _24to32(std::array<uint8_t, 3> byteArray)
{
    return (((int32_t)(byteArray[0]) << 24) |
            ((int32_t)(byteArray[1]) << 16) |
            ((int32_t)(byteArray[2]) << 8)) >> 8;
}

#ifdef CPYTHON
py::bytes
#else
char *
#endif
bytes_to_32(const char *buf, size_t len, bool invertEndianness)
{
    if (!buf || !len) return nullptr;
	
    int32_t *tmp_int_arr = new int32_t[len/3];
    std::array<uint8_t, 3> tmparr;
    std::string newbuf;

    for (size_t i = 0; i < len; i += 3)
    {
        tmparr[0] = (uint8_t) buf[i];
        tmparr[1] = (uint8_t) buf[i+1];
        tmparr[2] = (uint8_t) buf[i+2];
		
        *(tmp_int_arr + (i/3)) = _24to32(tmparr);
    }

    for (size_t i = 0; i < 4*len/3; i += 4)
    {
        newbuf += 0xFF & (tmp_int_arr[i/4] >> (invertEndianness ? 0 : 24));
        newbuf += 0xFF & (tmp_int_arr[i/4] >> (invertEndianness ? 8 : 16));
        newbuf += 0xFF & (tmp_int_arr[i/4] >> (invertEndianness ? 16 : 8));
        newbuf += 0xFF & (tmp_int_arr[i/4] >> (invertEndianness ? 24 : 0));
    }

    delete[] tmp_int_arr;

#ifdef CPYTHON
    return py::bytes(newbuf);
#else
    return newbuf.c_str();
#endif
}

#ifdef CPYTHON
py::bytes
#else
char *
#endif
invert_endianness_24(const char *buf, size_t len)
{
    if (!buf || !len) return nullptr;
	
    std::string newbuf;

    for (size_t i = 0; i < len; i += 3)
    {
        newbuf += (uint8_t) *(buf + i + 2);
        newbuf += (uint8_t) *(buf + i + 1);
        newbuf += (uint8_t) *(buf + i);
    }
    
#ifdef CPYTHON
    return py::bytes(newbuf);
#else
    return newbuf.c_str();
#endif
}

} // end namespace bit_converter
