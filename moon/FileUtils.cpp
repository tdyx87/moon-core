#include <moon/FileUtils.h>
#include <moon/FileInfo.h>

#include <libgen.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

namespace moon
{
namespace FileUtils
{

bool exists(const std::string &pathname)
{
	struct stat info;
	return ::stat(pathname.c_str(), &info) == 0;
}

bool remove(const std::string &pathname)
{
   return ::remove(pathname.c_str()) == 0;    
}

bool rename(const std::string &oldname, const std::string &newname)
{
	return ::rename(oldname.c_str(), newname.c_str()) == 0;
}

std::string dirname(const std::string &pathname)
{    
	char buf[PATH_MAX] = {0};
	memcpy(buf, pathname.c_str(), pathname.length());
	return std::string(::dirname(buf));
}

std::string baseName(const std::string &pathname)
{
	char buf[NAME_MAX] = {0};
	memcpy(buf, pathname.c_str(), pathname.length());
	return std::string(::basename(buf));
}


}  // ~FileUtils
}  // ~moon