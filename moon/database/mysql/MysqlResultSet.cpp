#include <moon/database/mysql/MysqlOps.h>
#include <moon/database/mysql/MysqlResultSet.h>

#include <assert.h>
#include <stdio.h>

namespace moon
{

namespace database
{

using std::string;

MysqlResultSet::MysqlResultSet(MYSQL_RES* res)
{
	mCurrentPos = -1;
	mColumnCounts = 0; 
	mRowCounts = 0;
    
	mMysqlRes = res;
}

MysqlResultSet::~MysqlResultSet()
{
	this->close();
}

void MysqlResultSet::close()
{
	if (NULL != mMysqlRes)
	{
		mysql::free_result(mMysqlRes);
		mMysqlRes = NULL;
	}
}

std::string MysqlResultSet::getString(const std::string& columnName) throw(SQLException)
{
	if (mFields.find(columnName) == mFields.end())
	{
		string err = "column = " + columnName + " does not appeared in results";
		throw(SQLException(err));
	}   
	
	assert(NULL != mCurrentRow);
    
	int index = mFields[columnName];
	return mCurrentRow[index] ? mCurrentRow[index] : "";
}

std::string MysqlResultSet::getString(unsigned int columnIndex) throw(SQLException)
{
	if (columnIndex >= mColumnCounts)
	{
		char buf[64] = {0};
		snprintf(buf, sizeof(buf), "column = %u is out of the range", columnIndex);
		throw(SQLException(buf));
	}

	assert(NULL != mCurrentRow);
 	return mCurrentRow[columnIndex] ? mCurrentRow[columnIndex] : "";
}

bool MysqlResultSet::first()
{
	if (0 == mCurrentPos)
	{
		return true;
	}
	return this->moveTo(0);
}

bool MysqlResultSet::last()
{
	if (mRowCounts - 1 == static_cast<unsigned long>(mCurrentPos))
	{
		return true;
	}

	return this->moveTo(mRowCounts - 1);
}

bool MysqlResultSet::next()
{
	return this->moveTo(mCurrentPos + 1);
}

bool MysqlResultSet::prev()
{
	return this->moveTo(mCurrentPos - 1);
}

bool MysqlResultSet::moveTo(unsigned long position)
{
	if (position >= mRowCounts)
	{
		return false;
	}
    
	mysql::data_seek(mMysqlRes, position);    
	mCurrentRow = mysql::fetch_row(mMysqlRes);
	if (NULL == mCurrentRow)
	{
		return false;
	}
    
	mCurrentPos = position;
	return true;
}

bool MysqlResultSet::init()
{
	assert(mMysqlRes != NULL);
	    
	mColumnCounts = mysql::num_fields(mMysqlRes);
	mRowCounts = mysql::num_rows(mMysqlRes);
	if (mRowCounts == 0)
	{
		return true;
	}
   
	// 获取列名称和列在结果集中的位置
	MYSQL_FIELD *fields = mysql::fetch_fields(mMysqlRes);
	for (unsigned int i = 0; i < mColumnCounts; ++i)
	{
		mFields[fields[i].name] = i;
	}
    
	mCurrentRow = mysql::fetch_row(mMysqlRes);
	if (NULL == mCurrentRow)
	{
		mRowCounts = 0;
		return false;
	}

	//mCurrentPos = 0;
	return true;
}


}  // ~database
}  // ~moon