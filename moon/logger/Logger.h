/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/

#ifndef MOON_LOGGER_LOGGER_H_
#define MOON_LOGGER_LOGGER_H_

#include <moon/logger/LoggerStream.h>
#include <moon/logger/Loggerfwd.h>
#include <string.h>
#include <string>

namespace moon
{

class Logger
{	
public:	
	enum Level
	{
	    LEVEL_TRACE,
		LEVEL_DEBUG,
		LEVEL_INFO,
		LEVEL_WARN,
		LEVEL_ERROR,
		LEVEL_FATAL,
		LEVEL_OFF,
	};

	Logger(Level level, const char *filename, int line);
	~Logger();
    
	static void setLevel(Level level) {sLevel = level;}
	static Level getLevel() {return sLevel;}
	
	LoggerStream& stream() {return mLoggerStream;}
	Level level() const {return mLevel;}
private:
	void logging();
private:
	static Level sLevel;
	static LoggingPolicyPtr sLoggingPolicy;

	const Level mLevel;
	LoggerStream mLoggerStream;
	const std::string mBasename;
	const int mLine;
};



}  // ~moon

static inline const char* sourceFileBaseName(const char *file)
{
	return strrchr(file, '/') ? (strrchr(file, '/') + 1) : file;
}

#define LOGGER_TRACE if (moon::Logger::getLevel() <= moon::Logger::LEVEL_TRACE) \
    moon::Logger(moon::Logger::LEVEL_TRACE, sourceFileBaseName(__FILE__), __LINE__).stream().logging
   
#define LOGGER_DEBUG if (moon::Logger::getLevel() <= moon::Logger::LEVEL_DEBUG) \
	moon::Logger(moon::Logger::LEVEL_DEBUG, sourceFileBaseName(__FILE__), __LINE__).stream().logging

#define LOGGER_INFO if (moon::Logger::getLevel() <= moon::Logger::LEVEL_INFO) \
	moon::Logger(moon::Logger::LEVEL_INFO, sourceFileBaseName(__FILE__), __LINE__).stream().logging

#define LOGGER_WARN if (moon::Logger::getLevel() <= moon::Logger::LEVEL_WARN) \
	moon::Logger(moon::Logger::LEVEL_WARN, sourceFileBaseName(__FILE__), __LINE__).stream().logging

#define LOGGER_ERROR if (moon::Logger::getLevel() <= moon::Logger::LEVEL_ERROR) \
	moon::Logger(moon::Logger::LEVEL_ERROR, sourceFileBaseName(__FILE__), __LINE__).stream().logging

#define LOGGER_FATAL if (moon::Logger::getLevel() <= moon::Logger::LEVEL_FATAL) \
	moon::Logger(moon::Logger::LEVEL_FATAL, sourceFileBaseName(__FILE__), __LINE__).stream().logging
			


#endif
