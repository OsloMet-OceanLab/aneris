#ifndef _24to32_HPP
#define _24to32_HPP

#pragma once

#include <cstdint>

static inline int32_t _24to32(std::array<uint8_t, 3> byteArray);
char* parseNum(char* buf, size_t len);

#endif
