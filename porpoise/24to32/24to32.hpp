#ifndef _24to32_HPP
#define _24to32_HPP

typedef char byte;

namespace _24bit_converter
{

byte* numto16(const byte *buf, size_t len, bool invertEndianness);
byte* numto32(const byte* buf, size_t len);

} // end namespace _24bit_converter

#endif
