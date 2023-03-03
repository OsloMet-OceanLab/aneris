#include <iostream>
#include <fstream>
#include "24to32.hpp"

int main()
{
    byte *buffer = (byte*)malloc(sizeof(byte)*960156);

    if (!buffer) exit(1);

    std::ifstream ifs("test", std::ios::in | std::ios::binary);
    ifs.read(buffer, 960156);
    ifs.close();

    printf("%02X %02X\n", (uint8_t)buffer[0], (uint8_t)buffer[1]);
    printf("%02X %02X\n", (uint8_t)buffer[960154], (uint8_t)buffer[960155]);

    //byte *buffer2 = (byte*)malloc(sizeof(byte)*999);

    //memcpy(buffer2, buffer+18, 999);

    //byte *buf3 = _24bit_converter::numto32(buffer2, 999);

    //for (int i = 0; i < 12; ++i)
        //printf("%c ", isprint(buf3[i]) ? buf3[i] : '.');

    //std::ofstream ofs("packet-32", std::ios::out | std::ios::binary);
    //ofs.write(buf3, 999/3*4);
    //ofs.close();

    byte *buf4 = _24bit_converter::numto32(buffer, 960156, true);
    std::ofstream ofs2("packet-32-little", std::ios::out | std::ios::binary);
    ofs2.write(buf4, 960156/3*4);
    ofs2.close();

    byte *buf5 = _24bit_converter::numto16(buffer, 960156, true);
    std::ofstream ofs3("test-16-little", std::ios::out | std::ios::binary);
    ofs3.write(buf5, 960156/3*2);
    ofs3.close();

    return 0;
}
