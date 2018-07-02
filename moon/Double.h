/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/

#ifndef MOON_DOUBLE_H_
#define MOON_DOUBLE_H_

#include <string>

namespace moon
{
using std::string;

/**
 * double行数据工具类
 */
class Double
{
public:
    // 将字符串str转换为一个double数据，如果字符串为空返回def指定的值
	static double parseDouble(const std::string& str, double def = dEPSINON);
    
	// 将字符串str转换为一个double数据，如果(str==null || strlen(str)==0)为空返回def指定的值
	static double parseDouble(const char* str, double def = dEPSINON);
	
	static std::string toString(double d);

	// 比较d1和d2
	// 返回1表示d1>d2, 返回0表示d1==d2,返回-1表示d1<d2
	static int compare(double d1, double d2);
public:
	static const double dEPSINON = 0.000001;  
	static const double MAX_VALUE;  // double能表示的最大值
	static const double MIN_VALUE;  // double能表示的最小值
};



}       //  ~moon
#endif  //  ~
