#include "Logger.hpp"
#include <fstream>
#include <ctime>
#include <string>
#include <cstdarg>

#define FILEPATH "/var/log/aneris/aneris.log"
#define F_BUFSIZE 256

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
	std::string strLevel = "[]";
	
	if(level & LOG_INFO) strLevel = "[INFO]";
	else if(level & LOG_WARN) strLevel = "[WARN]";
	else if(level & LOG_ERROR) strLevel = "[ERROR]";
	else if(level & LOG_FATAL) strLevel = "[FATAL]";

	// out flag allows to write, app flag allows to not overwrite file
	std::ofstream ofs(FILEPATH, std::ios_base::out | std::ios_base::app);
	ofs << strLevel << '\t' << getCurrentTime() << '\t' << logMsg << '\n';
	ofs.close();
}

void logf(long level, const char *fmt, ...)
{
	std::string strLevel = "[]";
	char logMsg[F_BUFSIZE];
	
	if(level & LOG_INFO) strLevel = "[INFO]";
	else if(level & LOG_WARN) strLevel = "[WARN]";
	else if(level & LOG_ERROR) strLevel = "[ERROR]";
	else if(level & LOG_FATAL) strLevel = "[FATAL]";

	va_list args;
	va_start(args, fmt);
	vsnprintf(logMsg, F_BUFSIZE, fmt, args);

	std::ofstream ofs(FILEPATH, std::ios_base::out | std::ios_base::app);
	ofs << strLevel << '\t' << getCurrentTime() << '\t' << logMsg << '\n';
	ofs.close();

	va_end(args);
}

void clearLog()
{
	std::ofstream ofs(FILEPATH, std::ios_base::out | std::ios_base::trunc);
	ofs.close();
	log(LOG_INFO, "Cleared out log file");
}

} // end namespace Logger
