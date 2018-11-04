#include <moon/database/mysql/MysqlConnection.h>
#include <moon/database/mysql/MysqlOps.h>

#include <assert.h>
#include <stdio.h>

namespace moon
{

namespace database
{

namespace mysql
{
using std::string;

MysqlConnection::MysqlConnection(MYSQL* pMysql, const std::string& name) : m_mysql(pMysql), m_name(name)
{
	if (NULL == m_mysql)
	{
		m_connected = false;
	}

	m_connected = true;
}

MysqlConnection::~MysqlConnection()
{
	this->close();
}

void MysqlConnection::close()
{
	if (NULL != m_mysql)
	{
		assert(m_connected);

		mysql::close(m_mysql);
		m_mysql = NULL;
		m_connected = false;
	}	
}

int MysqlConnection::ping()
{
	if (NULL == m_mysql)
	{
		return -1;
	}

	assert(m_connected);

	return mysql::ping(m_mysql);
}


}  //  ~mysql

}  //  ~database

}  //  ~moon

