/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/

#ifndef MOON_STRINGUTILS_H_
#define MOON_STRINGUTILS_H_

#include <string>

namespace moon
{
namespace StringUtils
{
using std::string;

const int kTrimLeft = 1;
const int kTrimRight = 2;
const int kTrimBothEnd = 4;

/**
  将s指向的字符串，从offset开始，将其后的count字节转换成小写。
  如果 (offset > strlen(s))，字符串s不会进行任何转换。
  如果count大于可转换的字节数，只转换offset后面的所有字节数。
  如果count==0，转换offset后面的所有字节数。
  转换操作是原址的，会直接在s上进行修改。
  
  @return 返回字符串s本身.
*/
char* toLowerCase(char* s, size_t count = 0, size_t offset = 0);
string& toLowerCase(string& s, size_t count = 0, size_t offset = 0);

char* toUpperCase(char* s, size_t count = 0, size_t offset = 0);
string& toUpperCase(string& s, size_t count = 0, size_t offset = 0);

/** 
  移除字符串src中所有的空白字符，包括字符串左端的空白字符，右端的空白字符以及字符串中间的空白字符。
  返回移除空白字符后的新字符串  
*/
string removeAllSpace(const std::string& src);

/**
  移除字符串src两端的空白字符。根据mode值不同进行不同操作。
  mode == kTrimLeft移除字符串左端的空白字符。
  mode == kTrimRight移除字符串右端的空白字符。
  mode == kTrimBothEnd移除字符串左右两端的空白字符。

  @return 返回移除空白字符后的新字符串
*/
string trimSpace(const std::string& src, int mode = kTrimBothEnd);


}  //~ util
}  //~ moon
#endif  //~ MOON_UTIL_STRING_UTILS_H_
