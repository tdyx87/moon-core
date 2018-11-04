#ifndef MOON_DATABASE_MYSQL_MYSQL_STATEMENT_H_
#define MOON_DATABASE_MYSQL_MYSQL_STATEMENT_H_

#include <moon/database/Statement.h>

#include <string>

#include <mysql/mysql.h>

namespace moon
{
namespace database
{

class MysqlStatement : public Statement
{
public:
	MysqlStatement(MYSQL* pMysql);
    virtual ~MysqlStatement();

	virtual int execute(const std::string& sql) throw(SQLException);
    
	virtual ResultSetPtr executeQuery(const std::string& sql) throw(SQLException);
private:
	MYSQL* mMysql;
};


}  // ~database
}  // ~moon

#endif  // ~MOON_DATABASE_MYSQL_MYSQL_STATEMENT_H_