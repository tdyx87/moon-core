#ifndef MOON_DATABASE_RESULTSET_H_
#define MOON_DATABASE_RESULTSET_H_

#include <moon/database/DatabaseDefine.h>
#include <moon/database/SQLException.h>
#include <boost/noncopyable.hpp>

#include <string>

namespace moon
{
namespace database
{

/**
* ResultSet类是一个抽象接口类，每个ResultSet子类的对象代表了一个从数据库查询（执行select语句）返回的结果集。
* ResultSet实现类有一个游标指向结果集中的行，当ResultSet对象被创建的时候该游标的位置指向第一行之前，即一个无效的位置。
* 使用next方法可将游标指向第一行数据或者下一行数据，只要未到达结果集最后一行，next方法将返回true，
* 当游标指向结果集最后一行时，再调用next方法将返回false。
* 
* 可使用getRowCount方法返回结果集的总行数，使用getColumnCount返回结果集每一行列的数量。
* 对每一行数据的访问支持2种方式，支持按列名称获取数据和列索引号获取数据。
* 当获取所有所需数据后可调用close方法释放系统资源，该方法不是必须的，在ResultSet对象销毁时
* 会自动调用close方法释放系统资源，为了能及时释放系统资源，建议用户在获取所需数据后尽快调用close方法释放系统资源。
*/
class ResultSet
{
public:
    ResultSet();
    virtual ~ResultSet();

	/**
	* 释放该对象持有的数据库资源，调用此方法后将无法再从该对象获取任何数据，
	* 调用之前请确认将不再从该对象获取任何数据。
	*/
	virtual void close() = 0;

    /**
    * 返回结果集行数量，返回0表示结果集为空
    */
	virtual unsigned long getRowCount() const = 0;    

	/**
	* 返回true表示结果集为空
	*/
	virtual bool isEmpty() const = 0;	

    /**
    * 返回结果集每行列的数量
    */
	virtual unsigned int getColumnCount()const = 0;	

    /**
    * 从当前行获取字段columnName列的数据，如果行中未包含字段columnName，将抛出SQLException异常
    * @param columnName 数据字段名
    * @throw SQLException 当字段名columnName不存在时抛出异常
    */
	virtual std::string getString(const std::string& columnName) throw(SQLException) = 0;

	/**
	* 从当前行获取第columnIndex列的数据，columnIndex取值范围为[0, getColumnCount()),
	* 如果columnIndex不在指定范围将抛出SQLException异常
	* @param columnIndex 列索引号，一个非负整数，取值范围为[0, getColumnCount())。
	* @throw SQLException 当columnIndex >= getColumnCount()时抛出异常
	*/
	virtual std::string getString(unsigned int columnIndex) throw(SQLException) = 0;   

	// 将当前游标指向结果集的第一行，如果结果集不为空返回true，结果集为空返回false.
	virtual bool first() = 0;

	// 将当前游标指向结果集的最后一行，如果结果集不为空返回true，结果集为空返回false.
	virtual bool last() = 0;
	virtual bool next() = 0;    
	virtual bool prev() = 0;
	virtual bool moveTo(unsigned long position) = 0;	
  
};  // ~ResultSet


}  // ~database
}  // ~moon

#endif  // 
