/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/

#ifndef MOON_LONG_H_
#define MOON_LONG_H_

#include <string>

namespace moon
{

/**
 * long工具类，提供了一些针对long型数据的处理方法
 */
class Long
{
public:
	static long parseLong(const std::string& str, long def = 0L, int radix = 10);
	static long parseLong(const char* str, long def = 0L, int radix = 10);

	static std::string toString(long l);
public:
	static const long MAX_VALUE;
	static const long MIN_VALUE;

};  //~ Long

}  //~ moon

#endif