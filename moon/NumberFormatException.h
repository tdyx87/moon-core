/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/

#ifndef MOON_NUMBERFORMATEXCEPTION_H_
#define MOON_NUMBERFORMATEXCEPTION_H_

#include <moon/Exception.h>

#include <string>

namespace moon
{

/**
 * 如果应用尝试将字符串数据转换为一种数值类型（int, float, double...），当字符串格式不合法时，抛出该异常。
 */
class NumberFormatException : public Exception
{
public:
	explicit NumberFormatException(const std::string& msg) : Exception(msg) {}
    
    static NumberFormatException forInputString(const std::string& str) 
	{
        return NumberFormatException("For input string: \"" + str + "\"");
    }
};


}  //~ namespace moon

#endif  //~ 
