#include <moon/logger/Logger.h>
#include <moon/logger/LoggingPolicy.h>
#include <moon/thread/CurrentThread.h>
#include <moon/Integer.h>
#include <moon/Timestamp.h>

#include <stdarg.h>
#include <stdio.h>
#include <map>
#include <string>

using std::string;

namespace moon {

const char* LogLevelName[Logger::LEVEL_OFF] =
{
  "TRACE ",
  "DEBUG ",
  "INFO  ",
  "WARN  ",
  "ERROR ",
  "FATAL ",
};

/*
LoggingItem::LoggingItem(Logger::Level level) : mLevel(level)
{
	const char *space = "  ";
	mLogStream << LogLevelName(level) << space;
	mLogStream << Timestamp::now().toMicroFormattedString() << space;
	mLogStream << CurrentThread::tidString() << space;
}
*/

Logger::Level Logger::sLevel = Logger::LEVEL_TRACE;
LoggingPolicyPtr Logger::sLoggingPolicy = LoggingPolicy::getSyncLoggingPolicy();


Logger::Logger(Level level, const char *filename, int line) : mLevel(level), mBasename(filename), mLine(line)
{
	mLoggerStream << Timestamp::now().toMicroFormattedString() << " ";
    mLoggerStream << CurrentThread::tidString()  << " ";
	mLoggerStream << LogLevelName[level] << " ";
}

Logger::~Logger()
{
	mLoggerStream << " - " << mBasename << ':' << mLine << '\n';
	logging();
}

void Logger::logging()
{
	if (sLoggingPolicy)
	{
	    sLoggingPolicy->logging(mLoggerStream.buffer());
	}
}



} // ~moon
