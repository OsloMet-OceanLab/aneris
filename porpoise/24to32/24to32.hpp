#ifndef _24to32_HPP
#define _24to32_HPP

#include <cstdint>
#include <array>

typedef char byte;

byte* _24to16(const byte *buf, size_t len, bool invertEndianess);
static inline int32_t _24to32(std::array<uint8_t, 3> byteArray);
byte* parseNum(byte* buf, size_t len);

#endif
