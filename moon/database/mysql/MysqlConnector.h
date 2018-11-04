#ifndef MOON_DATABASE_MYSQL_MYSQL_CONNECTOR_H_
#define MOON_DATABASE_MYSQL_MYSQL_CONNECTOR_H_


#include <string>
#include <mysql/mysql.h>

namespace moon
{

namespace database
{

namespace mysql
{

class MysqlConnector
{
public:
	MysqlConnector(const std::string& ip, const std::string& userName, const std::string& password,
	               const std::string& db, int port, const std::string& characterSet);
	MYSQL* connect();
    
    const std::string& getErrorInfo()const {return m_errorInfo;}
private:
	std::string m_ip;
    std::string m_userName;
	std::string m_password;
	std::string m_db;	
	int m_port;	
	std::string m_characterSet;

	std::string m_errorInfo;
};




}  //  ~mysql

}  //  ~database

}  //  ~moon


#endif  // ~MOON_DATABASE_MYSQL_MYSQL_CONNECTOR_H_