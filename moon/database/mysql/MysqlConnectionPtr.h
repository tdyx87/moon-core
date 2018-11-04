#ifndef MOON_DATABASE_MYSQL_MYSQL_CONNECTION_PTR_H_
#define MOON_DATABASE_MYSQL_MYSQL_CONNECTION_PTR_H_


#include <boost/shared_ptr.hpp>

namespace moon
{

namespace database
{

namespace mysql
{
class MysqlConnection;

typedef boost::shared_ptr<MysqlConnection> MysqlConnectionPtr;

}  //  ~mysql


}  //  ~database

}  //  ~moon



#endif  // ~MOON_DATABASE_MYSQL_MYSQL_CONNECTION_PTR_H_