/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/
#ifndef MOON_LOOGER_LOGGERSTREAM_H_
#define MOON_LOOGER_LOGGERSTREAM_H_

#include <moon/logger/LoggerBuffer.h>
#include <moon/logger/Loggerfwd.h>
#include <moon/Slice.h>

namespace moon
{
class LoggerStream
{
public:
    LoggerStream() : mBuffer(new LoggerBuffer(4096)) {}
	~LoggerStream() {}

    LoggerStream& operator<<(bool v)
	{
	    mBuffer->append(v ? "true" : "false", v ? 4 : 5);
		return *this;
	}
	
	LoggerStream& logging() {return *this;}
	LoggerStream& logging(const char* format, ...);	

	LoggerStream& operator<<(short);
    LoggerStream& operator<<(unsigned short);
    LoggerStream& operator<<(int);
    LoggerStream& operator<<(unsigned int);
    LoggerStream& operator<<(long);
    LoggerStream& operator<<(unsigned long);
    LoggerStream& operator<<(long long);
    LoggerStream& operator<<(unsigned long long);

    LoggerStream& operator<<(const void*);
	LoggerStream& operator<<(float v)
    {
        *this << static_cast<double>(v);
        return *this;
    }
    LoggerStream& operator<<(double);

	LoggerStream& operator<<(char v)
    {
        mBuffer->append(&v, 1);
        return *this;
    }

	LoggerStream& operator<<(const char* v)
    {
        mBuffer->append(v, strlen(v));
        return *this;
    }

    LoggerStream& operator<<(const std::string& v)
    {
        mBuffer->append(v.c_str(), v.size());
        return *this;
    }

	LoggerStream& operator<<(const Slice& s)
    {
        mBuffer->append(s.data(), s.size());
        return *this;
    }
 
	void append(const char* data, int len) { mBuffer->append(data, len); }
    
	LoggerBufferPtr buffer() const { return mBuffer; }
private:
	 template<typename T>  void formatInteger(T);
private:
	LoggerBufferPtr mBuffer;
};  // ~LoggerStream

}  // ~moon

#endif