/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/

#ifndef MOON_LOGGER_LOGGERBUFFER_H_
#define MOON_LOGGER_LOGGERBUFFER_H_

#include <stddef.h>

namespace moon{

class LoggerBuffer
{
public:
	LoggerBuffer(size_t size);
	~LoggerBuffer();

	LoggerBuffer(const LoggerBuffer &rhs);
	LoggerBuffer& operator=(const LoggerBuffer &rhs);

    const char* data() const {return mData;}

	size_t size() const {return mSize;}

	bool empty() const {return 0 == mSize;}

	size_t available() const {return mMaxSize - mSize - 1;}

	void add(size_t len) {mSize += len;}	

	void append(const char *buf);
	
	void append(const char *buf, size_t len);

	char* current() {return mData + mSize;}
private:	
	const size_t mMaxSize;
	char *mData;
	size_t mSize;
	
};  // ~LoggerBuffer


}  // ~moon


#endif