#ifndef MOON_FILEINFO_H_
#define MOON_FILEINFO_H_

#include <sys/stat.h>
#include <stdint.h>
#include <string>
#include <moon/Timestamp.h>

namespace moon
{

class FileInfo
{
public:	
	enum Permissions
	{
	    kOthExe     = 0x01,
        kOthWrite   = 0x02,
		kOthRead    = 0x04,
		kGroupExe   = 0x08,
		kGroupWrite = 0x10,
		kGroupRead  = 0x20,
		kOwnerExe   = 0x40,
		kOwnerWrite = 0x80,
	    kOwnerRead  = 0x0100		
    };

	FileInfo(const std::string &pathname);
    ~FileInfo();
	
	// 返回true表示该对象指向的文件存在，否则返回false。
    bool exists() const {return mExist;}

    // 返回一个字符串描述当前文件所属类型
	std::string fileTypeString() const;

	// 返回一个整数表示当前文件所属组的ID
	int groupId() const;

	// 返回true表示该对象指向一个文件夹，或者一个指向文件夹的符号链接。否则返回false。
	bool isDir() const;	

	// 返回true表示该对象指向一个块特殊文件，否则返回false。
	bool isBlockSpecialFile() const;
    
	// 返回true表示该对象指向一个字符特殊文件，否则返回false。
	bool isCharSpecialFile() const;    

	// 返回true表示该文件可执行
	bool isExecutable() const;		

	// 返回true表示该对象指向一个管道文件，否则返回false。
	bool isFIFO() const;
    
	// 返回true表示该对象指向一个文件，或者一个指向文件的符号链接。否则返回false。
	bool isFile() const;

	bool isPermission(int permissions) const;
    
	// 返回true表示该对象指向一个用于经常通信的套接字文件，否则返回false。
	bool isSocket() const;
    
	// 返回true表示该对象指向一个符号链接文件。否则返回false。
	bool isSymLink() const;

	// 返回一个整数表示文件最后一次修改的时间（单位:微秒）
	Timestamp lastModified() const;

	// 返回一个整数表示文件最后一次被访问（读取）的时间（单位:微秒）
	Timestamp lastRead() const;
    
	// 返回一个整数表示当前文件所属用户的ID
	int ownerId() const;
    
	std::string permissionString() const;
     
	/**
	  返回对象所指文件的的长度（以字节为单位）。
	  只对普通文件、目录以及符号链接有意义。
	  对于普通文件和目录一般返回文件的实际字节数。
	  对于符号链接返回所指实际文件的字节数，但是APUE说符号链接的长度是所指实际文件名称的长度，
	  和个人测试的不一致，还需要多测试。
	*/
	uint64_t size() const;	 
    
private:
	std::string mPathname;
	bool mExist;
	struct stat mInfo;
};



}  // ~moon

#endif