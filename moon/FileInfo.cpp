#include <moon/FileInfo.h>

#include <errno.h>
#include <stdio.h>
#include <string.h>

namespace moon
{

FileInfo::FileInfo(const std::string &pathname) : mPathname(pathname)
{
	mExist = (stat(pathname.c_str(), &mInfo) == 0);	
}

FileInfo::~FileInfo()
{
}

std::string FileInfo::fileTypeString() const
{
	if (isDir())
	{
		return "directory";
	}
	else if (isFile())
	{
		return "regular";
	}
	else if (isBlockSpecialFile())
	{
		return "block special";
	}
	else if (isCharSpecialFile())
	{
		return "character special";
	}
	else if (isFIFO())
	{
		return "fifo";
	}
	else if (isSocket())
	{
		return "socket";
	}
	else if (isSymLink())
	{
		return "symbolic link";
	}
	
	return "** unknown mode **";
}

int FileInfo::groupId() const
{
	return mInfo.st_gid;
}

bool FileInfo::isBlockSpecialFile() const
{
	return S_ISBLK(mInfo.st_mode);
}   

bool FileInfo::isCharSpecialFile() const   
{
	return S_ISCHR(mInfo.st_mode);
}

bool FileInfo::isDir() const
{
	return S_ISDIR(mInfo.st_mode);
}

bool FileInfo::isExecutable() const
{
	return false;
}

bool FileInfo::isFIFO() const   
{
	return S_ISFIFO(mInfo.st_mode);
}

bool FileInfo::isFile() const
{
	return S_ISREG(mInfo.st_mode);
}

bool FileInfo::isPermission(int permissions) const
{
	return mInfo.st_mode & permissions;
}

bool FileInfo::isSocket() const
{
	return S_ISSOCK(mInfo.st_mode);
}    

bool FileInfo::isSymLink() const
{
	return S_ISLNK(mInfo.st_mode);
}

Timestamp FileInfo::lastModified() const
{
	return Timestamp(static_cast<uint64_t>(mInfo.st_mtime) * 1000 * 1000);
}

Timestamp FileInfo::lastRead() const
{
	return Timestamp(static_cast<uint64_t>(mInfo.st_atime) * 1000 * 1000);
}

int FileInfo::ownerId() const
{
	return mInfo.st_uid;
}

std::string FileInfo::permissionString() const
{
	std::string str;   
	
	str += (mInfo.st_mode & S_IRUSR) ? "r" : "-";
	str += (mInfo.st_mode & S_IWUSR) ? "w" : "-";
	str += (mInfo.st_mode & S_IXUSR) ? "x" : "-";

	str += (mInfo.st_mode & S_IRGRP) ? "r" : "-";
	str += (mInfo.st_mode & S_IWGRP) ? "w" : "-";
	str += (mInfo.st_mode & S_IXGRP) ? "x" : "-";

	str += (mInfo.st_mode & S_IROTH) ? "r" : "-";
	str += (mInfo.st_mode & S_IWOTH) ? "w" : "-";
	str += (mInfo.st_mode & S_IXOTH) ? "x" : "-";

	return str;	
}

uint64_t FileInfo::size() const
{
	return static_cast<uint64_t>(mInfo.st_size);
}

}  // ~moon