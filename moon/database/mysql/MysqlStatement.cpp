#include <moon/database/mysql/MysqlOps.h>
#include <moon/database/mysql/MysqlResultSet.h>
#include <moon/database/mysql/MysqlStatement.h>

#include <assert.h>
#include <stdio.h>

namespace moon
{
namespace database
{
using std::string;

MysqlStatement::MysqlStatement(MYSQL* pMysql) : mMysql(pMysql)
{
}	

MysqlStatement::~MysqlStatement()
{
	mMysql = NULL;
}

int MysqlStatement::execute(const std::string& sql) throw(SQLException)
{
	assert(mMysql != NULL);

	if (sql.empty())
	{
		throw(SQLException("sql statement is empty"));
	}
    
	// execute sql statement
	if (mysql::real_query(mMysql, sql.c_str(), sql.length()) != 0)
	{
		throw(SQLException(string("execute:") + mysql::error(mMysql) + ", sql:" + sql));
	}
    
	// 对于insert, update, delete语句不会返回负数，如果执行了select语句可能返回负数。
	my_ulonglong ret = mysql::affected_rows(mMysql);
	if (static_cast<my_ulonglong>(-1) == ret)
	{
		return -1;
	}

	return static_cast<int>(ret);
}

ResultSetPtr MysqlStatement::executeQuery(const std::string& sql) throw(SQLException)
{
	assert(mMysql != NULL);

	if (sql.empty())
	{
		throw(SQLException("sql statement is empty"));
	}

	// execute sql statement
	if (mysql::real_query(mMysql, sql.c_str(), sql.length()) != 0)
	{
		throw(SQLException(string("executeQuery:") + mysql::error(mMysql) + ", sql:" + sql));	
	}

	MYSQL_RES* res = mysql::store_result(mMysql);
	if (NULL == res)
	{
		throw(SQLException(string("store_result error: ") + mysql::error(mMysql)));	
	}

    MysqlResultSet * result = new MysqlResultSet(res);
	if (!result->init())
	{
		throw(SQLException(string("init result sets: ") + mysql::error(mMysql)));	
	}

	return ResultSetPtr(result);
}



}  // ~database
}  // ~moon