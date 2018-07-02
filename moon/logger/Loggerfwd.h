/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/
#ifndef MOON_LOGGER_LOGGERFWD_H_
#define MOON_LOGGER_LOGGERFWD_H_

#include <boost/shared_ptr.hpp>
namespace moon {

class LoggingPolicy;
typedef boost::shared_ptr<LoggingPolicy> LoggingPolicyPtr;

class LoggerBuffer;
typedef boost::shared_ptr<LoggerBuffer> LoggerBufferPtr;


	
}  // ~moon
#endif
