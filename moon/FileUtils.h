#ifndef MOON_FILEUTILS_H_
#define MOON_FILEUTILS_H_

#include <string>

namespace moon
{
namespace FileUtils
{
bool clearFile(const std::string &pathname);

/**
  返回true表示pathname路径指向的文件/目录存在，否则返回false。
*/
bool exists(const std::string &pathname);

/**
  返回true表示删除pathname路径指向的文件/目录成功，否则返回false。
  当pathname表示目录时，只能删除空目录（只包含.和..），如果目录不为空，则无法删除目录。
*/
bool remove(const std::string &pathname);

/**
  将oldname指向的文件/目录名重命名为newname。

  1 如果oldname指向一个非目录文件：
    1.1 如果newname不存在，则将该文件更名为newname。
	1.2 如果newname指向一个非目录文件，先删除newname指向的文件，然后将该文件更名为newname。
	1.3 如果newname指向一个目录文件，则更名失败。

  2 如果oldname指向的是一个目录：
    2.1 如果newname不存在，则将目录名称更名为newname。
	2.2 如果newname存在且指向一个空目录，那么先将newname指向的空目录删除，然后将目录名称更名为newname。
    2.3 如果newname存在且指向一个非空目录，那么更名失败。
	2.4 如果newname存在且指向一个非目录文件，那么更名失败
  
  如果newname不存在，那么newname路径中的所有父目录必须存在，否则将更名失败。
  也就是说在更名的过程中，该方法不会自动创建缺失的目录。
  @return 返回true表示更名成功，失败返回false。
*/
bool rename(const std::string &oldname, const std::string &newname);

std::string dirname(const std::string &pathname);

std::string baseName(const std::string &pathname);



}  // ~FileUtils
}  // ~moon

#endif