#ifndef MOON_DATABASE_MYSQL_MYSQL_RESULT_SET_H_
#define MOON_DATABASE_MYSQL_MYSQL_RESULT_SET_H_

#include <moon/database/ResultSet.h>

#include <mysql/mysql.h>

#include <map>
#include <string>

namespace moon
{

namespace database
{

class MysqlResultSet : public ResultSet
{
public:
	friend class MysqlStatement;
    MysqlResultSet(MYSQL_RES* res);
    virtual ~MysqlResultSet();
    
	virtual void close();

	virtual unsigned long getRowCount()const {return mRowCounts;}    
    
	virtual bool isEmpty()const {return mRowCounts == 0;}	

	virtual unsigned int getColumnCount()const {return mColumnCounts;}	

	virtual std::string getString(const std::string& columnName) throw(SQLException);
	virtual std::string getString(unsigned int columnIndex) throw(SQLException);   
   	 
	virtual bool first();
	virtual bool last();
	virtual bool next();    
	virtual bool prev();
	virtual bool moveTo(unsigned long position);	
private:	
    bool init(); 
private:    
	MYSQL_RES* mMysqlRes;  
    MYSQL_ROW mCurrentRow;

	int mCurrentPos;            
	unsigned int mColumnCounts;  
	unsigned long mRowCounts;    

	std::map<std::string, unsigned int> mFields;  
};  // ~ResultSet



}  // ~database
}  // ~moon

#endif  // ~MOON_DATABASE_MYSQL_MYSQL_RESULT_SET_H_