/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/
#ifndef MOON_IOEXCEPTION_H_
#define MOON_IOEXCEPTION_H_

#include <moon/Exception.h>

namespace moon
{

class IOException : public Exception
{
public:	
	/**
    * 函数描述:构造函数
	* 函数参数:  msg: 详细信息
    */
	explicit IOException(const std::string& msg) : Exception(msg) {}
    
};

}  // ~moon

#endif