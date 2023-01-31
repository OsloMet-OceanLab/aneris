#include "Logger.hpp"
#include <fstream>
#include <time.h>
#include <string>

namespace Logger
{

static std::string getCurrentTime()
{
	time_t now = time(0);
	struct tm tstruct = *localtime(&now);
	char buf[40];
	strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
	return std::string(buf);
}

void log(long level, std::string logMsg)
{
	std::string filepath = "/home/pi/Desktop/log.txt", strLevel = "";
	
	if(level & LOG_INFO) strLevel = "[INFO]";
	else if(level & LOG_WARNING) strLevel = "[WARNING]";
	else if(level & LOG_ERROR) strLevel = "[ERROR]";
	else if(level & LOG_FATAL) strLevel = "[FATAL]";

	// out flag allows to write, app flag allows to not overwrite file
	std::ofstream ofs(filepath.c_str(), std::ios_base::out | std::ios_base::app);
	ofs << strLevel << '\t' << getCurrentTime() << '\t' << logMsg << '\n';
	ofs.close();
}

} // end namespace Logger
