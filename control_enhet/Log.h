#ifndef LOG_H
#define LOG_H

#include <string>
#include <fstream>
#include <time.h>

namespace Logger
{

#define LOG_INFO		0x1 // general information
#define LOG_WARNING		0x2 // can cause oddities
#define LOG_ERROR		0x4 // fatal to an operation but not a program
#define LOG_FATAL		0x8 // fatal to the whole program

static inline std::string getCurrentDateTime()
{
	time_t now = time(0);
	struct tm tstruct;
	char buf[80];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
	return std::string(buf);
}

inline void log(std::string logMsg, long level)
{
	std::string filepath = "/home/pi/Desktop/log.txt", strLevel = "";
	if(level & LOG_INFO) strLevel = "[INFO]";
	else if(level & LOG_WARNING) strLevel = "[WARNING]";
	else if(level & LOG_ERROR) strLevel = "[ERROR]";
	else if(level & LOG_FATAL) strLevel = "[FATAL]";
	std::string time = getCurrentDateTime();
	std::ofstream ofs(filepath.c_str(), std::ios_base::out | std::ios_base::app);
	ofs << strLevel << '\t' << time << '\t' << logMsg << '\n';
	ofs.close();
}

}
#endif
