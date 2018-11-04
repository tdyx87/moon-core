#include <moon/database/mysql/MysqlConnector.h>
#include <moon/database/mysql/MysqlOps.h>

namespace moon
{

namespace database
{

namespace mysql
{
using std::string;

MysqlConnector::MysqlConnector(const std::string& ip, const std::string& userName, const std::string& password,
			   const std::string& db, int port, const std::string& characterSet)
    : m_ip(ip),
	 m_userName(userName),
	 m_password(password),
	 m_db(db),
	 m_port(port),
	 m_characterSet(characterSet)
{
}

MYSQL* MysqlConnector::connect()
{
	MYSQL* pMysql = NULL;
	
	// init connection handle
	if ( (pMysql = mysql::init(NULL)) == NULL)
	{
		m_errorInfo = "init failed";
		return NULL;
	}
	
	// connect database
	if (mysql::real_connect(pMysql, m_ip, m_userName, m_password, m_db, m_port, NULL, 0) == NULL)
	{
		m_errorInfo = string("real_connect failed: ") + mysql::error(pMysql);
		return NULL;		
	} 
    
	if (mysql::set_character_set(pMysql, m_characterSet) != 0)
	{
		mysql::close(pMysql);
		m_errorInfo = string("set_character_set failed: ") + mysql::error(pMysql);
		return NULL;
	}

	return pMysql;
}

}  //  ~mysql

}  //  ~database

}  //  ~moon