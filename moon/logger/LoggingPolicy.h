/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/
#ifndef MOON_LOGGER_LOGGINGPOLICY_H_
#define MOON_LOGGER_LOGGINGPOLICY_H_

#include <moon/logger/Loggerfwd.h>
#include <boost/shared_ptr.hpp>
#include <stdio.h>

namespace moon {

class LoggingPolicy
{
public:
	virtual ~LoggingPolicy(){}

	static LoggingPolicyPtr getSyncLoggingPolicy();
	
	virtual void logging(const LoggerBufferPtr &buffer) = 0;		
};



	
}  // ~moon
#endif