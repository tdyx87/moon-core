#ifndef MOON_DATABASE_SQLEXCEPTION_H_
#define MOON_DATABASE_SQLEXCEPTION_H_

#include <moon/Exception.h>
#include <string>

namespace moon
{
namespace database
{

 
/**
* 抛出该异常表示执行sql语句失败，或者相关无效数据库操作
*/
class SQLException : public Exception
{
public:
	/**
    * 函数描述:构造函数
	* 函数参数:  msg: 详细信息
    */
	explicit SQLException(const std::string& msg) : Exception(msg) {}
   
};

}  // ~database
}  //  ~moon

#endif  //  ~end of file