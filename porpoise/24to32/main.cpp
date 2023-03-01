#include <iostream>
#include <fstream>
#include <array>
#include <cstdint>
#include <vector>

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
        *(newbuf + i) =       0xFF & (tmparr2[i/4] >> 24);
        *(newbuf + i + 1) =   0xFF & (tmparr2[i/4] >> 16);
        *(newbuf + i + 2) =   0xFF & (tmparr2[i/4] >> 8);
        *(newbuf + i + 3) =   0xFF & (tmparr2[i/4]);
    }
    return newbuf;
}

byte* _24to16(const byte* buf, size_t len, bool invertEndianess)
{
    std::vector<int16_t> tmparr2;

    byte* newbuf = (byte*)malloc(sizeof(byte)*2*len/3);

    for (size_t i = 0; i < len; i += 3)
        tmparr2.push_back( invertEndianess ?
                          (((uint16_t) (buf[i+2] << 8)) | (uint8_t) buf[i+1]) :
                          (((uint16_t) (buf[i+1] << 8)) | (uint8_t) buf[i+2])
                          );

    for (size_t i = 0; i < 2*len/3; i += 2)
    {
        *(newbuf + i) =       0xFF & (tmparr2[i/2] >> 8);
        *(newbuf + i + 1) =   0xFF & (tmparr2[i/2]);
    }
    return newbuf;
}

int main()
{
    std::ifstream ifs("packet", std::ios::in | std::ios::binary);

    byte *buffer = (byte*)malloc(sizeof(byte)*1017);

    ifs.read(buffer, 1017);
    ifs.close();

    printf("%02X %02X\n", (uint8_t)buffer[0], (uint8_t)buffer[1]);

    byte *buffer2 = (byte*)malloc(sizeof(byte)*999);

    memcpy(buffer2, buffer+18, 999);

    byte *buf3 = parseNum(buffer2, 999);

    for (int i = 0; i < 12; ++i)
        printf("%c ", isprint(buf3[i]) ? buf3[i] : '.');

    std::ofstream ofs("packet-32", std::ios::out | std::ios::binary);
    ofs.write(buf3, 999/3*4);
    ofs.close();

    byte *buf4 = _24to16(buffer2, 999, false);
    std::ofstream ofs2("packet-16", std::ios::out | std::ios::binary);
    ofs2.write(buf4, 999/3*2);
    ofs2.close();

    byte *buf5 = _24to16(buffer2, 999, true);
    std::ofstream ofs3("packet-16-little", std::ios::out | std::ios::binary);
    ofs3.write(buf5, 999/3*2);
    ofs3.close();

    return 0;
}
