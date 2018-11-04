#ifndef MOON_DATABASE_MYSQL_MYSQL_OPS_H_
#define MOON_DATABASE_MYSQL_MYSQL_OPS_H_


#include <mysql/mysql.h>

#include <string>

namespace moon
{

namespace database
{

namespace mysql
{

/**
    Returns the number of the rows that has been affected by the last sql statement.
*/
inline my_ulonglong affected_rows(MYSQL *mysql){return ::mysql_affected_rows(mysql);}


// TODO
//inline mybool autocommit(); 

// TODO
//inline my_bool change_user();  

// TODO
//inline const char* character_set_name();  

inline void close(MYSQL *mysql){::mysql_close(mysql);}

/**
*/
inline my_bool commit(MYSQL *mysql){return ::mysql_commit(mysql);}

// Deprecated, replaced by using mysql_real_connect()
//inline connect();  

// TODO
//inline int create_db();  

/**
*/
inline void data_seek(MYSQL_RES* res, unsigned long offset){	::mysql_data_seek(res, offset);}

// TODO
//inline void debug();  

// Deprecated, replaced by using mysql_query()
//inline int drop_db();  

// TODO
//inline int dump_debug_info();

// Deprecated, replaced by using mysql_errno() or mysql_error() 
//inline my_bool eof();

/**  
    Returns the error code of the last call, if there is no errors, returns 0
*/
inline unsigned int errno(MYSQL *mysql){	return ::mysql_errno(mysql);}


/**
    Retruns a pointer to the c-string representation of the error.
if there is no error occurs, returns a empty c-string
*/
inline const char* error(MYSQL *mysql){return ::mysql_error(mysql);}

//
inline MYSQL_FIELD* fetch_field(MYSQL_RES* res){	return ::mysql_fetch_field(res);}
inline MYSQL_FIELD* fetch_field_direct(MYSQL_RES* res, unsigned fieldrn){return ::mysql_fetch_field_direct(res, fieldrn);}

//
inline MYSQL_FIELD* fetch_fields(MYSQL_RES* res){return ::mysql_fetch_fields(res);}

//
inline MYSQL_ROW fetch_row(MYSQL_RES* res){return ::mysql_fetch_row(res);}

//
inline unsigned int field_count(MYSQL *mysql){return ::mysql_field_count(mysql);}

//
inline MYSQL_FIELD_OFFSET field_seek(MYSQL_RES* res, MYSQL_FIELD_OFFSET offset){return ::mysql_field_seek(res, offset);}

//
inline MYSQL_FIELD_OFFSET field_tell(MYSQL_RES* res){return ::mysql_field_tell(res);}

//
inline void free_result(MYSQL_RES* res){return ::mysql_free_result(res);}

//
inline MYSQL* init(MYSQL *mysql){return ::mysql_init(mysql);}

// 
inline unsigned int num_fields(MYSQL_RES* res){return ::mysql_num_fields(res);}

//
inline unsigned long num_rows(MYSQL_RES* res){return ::mysql_num_rows(res);}

//
inline int options(MYSQL *mysql, enum mysql_option option, const char *arg){return ::mysql_options(mysql, option, arg);}

//
inline int ping(MYSQL *mysql){return ::mysql_ping(mysql);}

//
inline int query(MYSQL *mysql, const char *query){return ::mysql_query(mysql, query);}

// 
inline MYSQL* real_connect(MYSQL *mysql, const std::string& host, const std::string& user, 
const std::string& passwd, const std::string& db, unsigned int port, const char *unix_socket, unsigned long client_flag)
{
	return ::mysql_real_connect(mysql, host.c_str(), user.c_str(), passwd.c_str(), db.c_str(), port, unix_socket, client_flag);
}

//
inline int real_query(MYSQL *mysql, const char *query, unsigned long length) {return ::mysql_real_query(mysql, query, length);}

//
inline MYSQL_ROW_OFFSET row_seek(MYSQL_RES *res, MYSQL_ROW_OFFSET offset){return ::mysql_row_seek(res, offset);}

//
inline MYSQL_ROW_OFFSET row_tell(MYSQL_RES *res){return mysql_row_tell(res);}

//
inline int set_character_set(MYSQL *mysql, const std::string& csname){return ::mysql_set_character_set(mysql, csname.c_str());}

//
inline MYSQL_RES* store_result(MYSQL *mysql){return ::mysql_store_result(mysql);}



}  //  ~mysql

}  //  ~database

}  //  ~moon


#endif  // ~MOON_DATABASE_MYSQL_MYSQL_OPS_H_