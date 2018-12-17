#ifndef MOON_DIRECTORY_H_
#define MOON_DIRECTORY_H_

#include <string>
#include <vector>

namespace moon
{

class Directory
{
public:
	 /**
      返回path对应的路径，除去path中多余的"/"，解析path中的"."和".."，将path转换成相对规范化的路径。
      @example: 
        1 "./local"转换成"local";
        2 "local/../bin/"转换成"bin";
        3 "/local/usr/../bin"转换成"/local/bin";
        4 "/local/usr/../../bin"转换成"/bin";
      如果path无法进行转换则返回一个空字符串，例如："/local/usr/../../../bin"无法进行转换。

      @return: 返回一个非空字符串对象表示转换后的路径，如果path无法进行转换返回一个空字符串。
    */
    static std::string cleanPath(const std::string &path);
   
    /**
      返回当前程序的工作目录。一般情况下该方法返回启动程序所在的目录，而不是程序执行文件所在的目录。
    */
    static std::string currentPath();

	/**
	  获取pathname目录下
	*/
    static int entryList(const std::string &pathname, std::vector<std::string> &filelists);
    
    /**
      返回true表示name对应的文件或者目录存在
    */
    static bool exists(const std::string &name);
     
    /**
      返回true表示path表示的路径为绝对路径
    */
    static bool isAbsolutePath(const std::string &path);
    
    /**
      创建由dirName指定的目录。dirName指定的父目录必须存在，该方法不会创建dirName路径中包含的父目录。
      如果dirName指定的目录已存在，返回false。
      @return 成功返回true，其它返回false.
    */
    static bool mkdir(const std::string &dirName);
    
    /**
      创建pathName指定的路径，会自动创建路径中缺失的所有父目录。如果pathName路径已存在返回true.
      @return 成功返回true，其它返回false.
    */
    static bool mkpath(const std::string &pathName);

    /**
      返回path路径对应的父路径，如果找不到path对应的父路径则返回一个空字符串。
      @example:
        1 "/local/usr/lib"的父路径为"/local/usr";
        2 "/local"的父路径为"/"；
        3 "/", "local", "local/", 这些路径无父路径。
    */
    static std::string parentPath(const std::string &path);

    static int setMode(int mode)
    {
    	  int preMode = sDefaultMode;
        sDefaultMode = mode;
    	  return preMode;
    }
private:
	static int sDefaultMode;  // 目录文件创建默认权限
};


}  //~ moon

#endif