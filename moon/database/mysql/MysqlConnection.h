#ifndef MOON_DATABASE_MYSQL_MYSQL_CONNECTION_H_
#define MOON_DATABASE_MYSQL_MYSQL_CONNECTION_H_

#include <string>

#include <mysql/mysql.h>
#include <boost/noncopyable.hpp>

namespace moon
{

namespace database
{

namespace mysql
{
class MysqlConnection : public boost::noncopyable
{
public:
	MysqlConnection(MYSQL* pMysql, const std::string& name=std::string());    
	~MysqlConnection();  
	
	/** close the connection */
	void close();    
	    
	/** 
	Returns true if the connection has established.
	*/
	bool isConnected()const {return m_connected;}   

	/**
	*/
    MYSQL* getMysql()const {return m_mysql;}

	const std::string& getName()const {return m_name;}

	/**
	@description:
        Checking whether this connection is valid or not.
	This method may be used for the connection that has idled for a long time, which can check
	that if the server has closed this connection, and can reconnection when necessary
	@return:
	    On success, returns 0, on error, returns -1
	*/
	int ping();    

private:
	MYSQL* m_mysql; 
    bool m_connected;  
	std::string m_name;
};  // ~MysqlConnection




}  //  ~mysql

}  //  ~database

}  //  ~moon

#endif  // ~MOON_DATABASE_MYSQL_MYSQL_CONNECTION_H_