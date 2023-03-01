#include <iostream>
#include <fstream>
#include <array>
#include <cstdint>

typedef char byte;

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
        printf("%d\n", tmpint);

        newbuf[i] =     (byte) (tmpint & (0xFF << 24));
        newbuf[i+1] =   (byte) (tmpint & (0xFF << 16));
        newbuf[i+2] =   (byte) (tmpint & (0xFF << 8));
        newbuf[i+3] =   (byte) (tmpint & (0xFF << 0));
    }
    return newbuf;
}

int main()
{
    std::ifstream ifs("packet", std::ios::in | std::ios::binary);

    byte *buffer = (byte*)malloc(sizeof(byte)*1017);

    ifs.read(buffer, 1017);

    printf("%02X %02X\n", (uint8_t)buffer[0], (uint8_t)buffer[1]);

    byte *buffer2 = (byte*)malloc(sizeof(byte)*999);

    memcpy(buffer2, buffer, 999);

    byte *buf3 = parseNum(buffer2, 999);

    for (int i = 0; i < 12; ++i)
        printf("%02X ", (uint8_t)buf3[i]);

    return 0;
}
