#ifndef LOG_H
#define LOG_H

#include <string>
#include <fstream>
#include <time.h>

namespace Logger
{

enum
{
	LOG_INFO =			0x1, // general information
	LOG_WARNING =		0x2, // can cause oddities
	LOG_ERROR =			0x4, // fatal to an operation but not a program
	LOG_FATAL =			0x8 // fatal to the whole program
};

static std::string getCurrentTime()
{
	time_t now = time(0);
	struct tm tstruct = *localtime(&now);
	char buf[40];
	strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
	return std::string(buf);
}

void log(std::string logMsg, long level)
{
	std::string filepath = "/home/pi/Desktop/log.txt", strLevel = "";
	
	if(level & LOG_INFO) strLevel = "[INFO]";
	else if(level & LOG_WARNING) strLevel = "[WARNING]";
	else if(level & LOG_ERROR) strLevel = "[ERROR]";
	else if(level & LOG_FATAL) strLevel = "[FATAL]";

	std::ofstream ofs(filepath.c_str(), std::ios_base::out | std::ios_base::app);
	ofs << strLevel << '\t' << getCurrentTime() << '\t' << logMsg << '\n';
	ofs.close();
}

} // end namespace Logger

#endif
