/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/

#ifndef MOON_EXCEPTION_H_
#define MOON_EXCEPTION_H_

#include <exception>
#include <string>

namespace moon
{

/**
 * 异常类，程序使用该类以及该类的子类抛出一个异常来表示程序正在执行一个不合法的操作。
 * 注意：
 * 1 不要在构造函数里面和析构函数里面使用异常。
 */
class Exception : public std::exception
{
public:
	explicit Exception(const char* what);
    explicit Exception(const std::string& what);
	virtual ~Exception() throw();

	// 返回异常提示信息
	virtual const char* what() const throw();

	// 返回异常时线程调用栈信息
	const char* stackTrace() const throw();
private:
    void fillStackTrace();
    std::string demangle(const char* symbol); //反名字改编
private:
	std::string mMessage;  // 异常信息
    std::string mStack;    // 异常上下文信息
};

}  //~ moon

#endif  //~ 
