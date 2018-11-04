#ifndef MOON_DATABASE_STATEMENT_H_
#define MOON_DATABASE_STATEMENT_H_

#include <moon/database/DatabaseDefine.h>
#include <moon/database/SQLException.h>

#include <string>

namespace moon
{
namespace database
{

class ResultSet;

class Statement
{
public:
	Statement();
    virtual ~Statement();

	/**
	* 执行sql语句，适用于insert, update, delete语句，不建议使用该方法执行select语句。
	* @param sql 一个表示sql语句的字符串，为空时抛出SQLException 
	* @return 成功返回一个非负整数，表示执行该sql语句被影响的行数。
	* 对于insert语句，返回值表示新插入的行数量.
	* 对于update语句，返回值表示被更新的行数量.
	* 对于delete语句，返回值表示被删除的行数量.
	* 对于其他sql语句，例如"begin", "commit", "rollback"返回0表示成功。
	* 返回负数表示执行失败。
	* @throw SQLException sql为空或者底层数据库执行失败抛出该异常
	*/
	virtual int execute(const std::string& sql) throw(SQLException) = 0;
    
	/**
	* 该方法用于select语句，不建议执行select以外的语句。返回一个ResultSetPtr对象，
	* ResultSetPtr是一个指向查询结果集的智能指针，当执行失败时返回一个空的智能指针.
	* @param sql 一个表示sql语句的字符串，为空时抛出SQLException 
	* @return 成功返回一个指向查询结果集的ResultSetPtr对象，执行失败返回一个空的ResultSetPtr对象。
    * @throw SQLException sql为空或者底层数据库执行失败抛出该异常
	*/
	virtual ResultSetPtr executeQuery(const std::string& sql) throw(SQLException) = 0;
};


}  // ~database
}  // ~moon

#endif  // ~MOON_DATABASE_STATEMENT_H_