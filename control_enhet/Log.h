#ifndef LOG_H
#define LOG_H

#include <string>
#include <fstream>
#include <time.h>

#define LOG_INFO		0x1
#define LOG_WARNING		0x2
#define LOG_ERROR		0x4

static inline std::string getCurrentDateTime(std::string s)
{
	time_t now = time(0);
	struct tm tstruct;
	char buf[80];
	tstruct = *localtime(&now);
	if(s == "now") strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
	else if (s == "date") strftime(buf, sizeof(buf), "%Y-%m-%d", &tstruct);
	return std::string(buf);
}

inline void log(std::string logMsg, long level)
{
	std::string filepath = "/home/pi/Desktop/log.txt", strLevel = "";
	if(level & LOG_INFO) strLevel = "[INFO]";
	if(level & LOG_WARNING) strLevel = "[WARNING]";
	if(level & LOG_ERROR) strLevel = "[ERROR]";
	std::string now = getCurrentDateTime("now");
	std::ofstream ofs(filepath.c_str(), std::ios_base::out | std::ios_base::app);
	ofs << strLevel << '\t' << now << '\t' << logMsg << '\n';
	ofs.close();
}

#endif
