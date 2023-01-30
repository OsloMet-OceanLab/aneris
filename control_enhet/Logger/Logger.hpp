#ifndef LOG_H
#define LOG_H

#include <string>

namespace Logger
{

enum
{
	LOG_INFO =		0x1, // general information
	LOG_WARNING =	0x2, // can cause oddities
	LOG_ERROR =		0x4, // fatal to an operation but not a program
	LOG_FATAL =		0x8 // fatal to the whole program
};

static std::string getCurrentTime();
void log(long level, std::string logMsg);

} // end namespace Logger

#endif
