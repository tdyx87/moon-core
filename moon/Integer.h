/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/

#ifndef MOON_INTEGER_H_
#define MOON_INTEGER_H_

#include <string>

namespace moon
{
using std::string;

/**
 * 整数工具类，提供了一些针对整数处理的方法
 * 1 parseInt方法低层使用了strtol方法，使用方式类似strtol
 */
class Integer
{
public:
	static int parseInt(const std::string& str, int def = 0, int radix = 10);
	static int parseInt(const char* str, int def = 0, int radix = 10);

	static std::string toString(int i);

public:
	static const int MAX_VALUE;  // int能表示的最大值
	static const int MIN_VALUE;  // int能表示的最小值


};  //~ Integer
}   //~ moon
#endif  //~ 
