#ifndef LOG_HPP
#define LOG_HPP

#include <string>
#include <cstdarg>

namespace Logger
{

enum
{
	LOG_INFO =		0x1, // general information (e.g. everything works)
	LOG_WARN =		0x2, // can cause oddities (e.g. can't ping client on land)
	LOG_ERROR =		0x4, // fatal to an operation but not a program (e.g. gpio is inaccessible)
	LOG_FATAL =		0x8  // fatal to the whole program (e.g. can't start the web server)
};

void log(long level, std::string logMsg);
void logf(long level, const char *fmt, ...);
void clearLog();

} // end namespace Logger

#endif
