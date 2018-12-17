#include <moon/Directory.h>
#include <moon/FileInfo.h>

#include <dirent.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

namespace moon
{
using std::string;
using std::vector;

static int getDefaultMode()
{
	int mode = 0;

	mode |= FileInfo::kOwnerRead | FileInfo::kOwnerWrite;
	mode |= FileInfo::kGroupRead | FileInfo::kGroupExe;
	mode |= FileInfo::kGroupRead | FileInfo::kGroupExe;
	return mode;  // 644权限
}

int Directory::sDefaultMode = getDefaultMode();

std::string Directory::cleanPath(const std::string &path)
{
    if (path.empty()) 
    {
        return "";
    }

    vector<string> slices;
    size_t start = 0;
    size_t next = path.find('/', start); 
    while (string::npos != next)
    {
    	std::string name = path.substr(start, next - start);
    	if (!name.empty())
    	{
            slices.push_back(name);
    	}
        start = next + 1;
        next = path.find('/', start);
    }

    std::string name = path.substr(start);
	if (!name.empty())
	{
        slices.push_back(name);
	}	

    if (slices.size() == 0)
    {
        return "";
    }

    if (".." == slices[0])
    {
        return "";
    }
    
    const bool isAbsolutePath = (path[0] == '/');
    vector<string> newSlices;
    for (size_t i= 0; i < slices.size(); ++i)
    {
        if ("." == slices[i])
        {
            // do nothing
        }
        else if (".." == slices[i])
        {
            if (newSlices.size() == 0)
            {
                return "";
            }
            newSlices.pop_back();
            continue;
        }
        else
        {
        	newSlices.push_back(slices[i]);
        }
    }

    std::string newPath = isAbsolutePath ? "/" : "";
    for (size_t i= 0; i < newSlices.size(); ++i)
    {
    	newPath += newSlices[i] + "/";
    }

    return newPath.substr(0, newPath.length() - 1);
}

string Directory::currentPath()
{
	char buf[PATH_MAX] = {0};
	return string( ::getcwd(buf, sizeof(buf)) );
}

int Directory::entryList(const string &pathname, std::vector<std::string> &filelists)
{
	DIR *dir = opendir(pathname.c_str());
	if (NULL == dir)
	{
		return -1;
	}
    
	struct dirent *entry = new dirent();
	struct dirent *result = new dirent();    
	int count = 0;

	while (1)
	{
		if (readdir_r(dir, entry, &result) != 0)
		{
			break;
		}

		if (NULL == result)
		{
			break;
		}

		if ('.' == result->d_name[0])
		{
			continue;
		}

		filelists.push_back(result->d_name);
		++count;
	}

	closedir(dir);
	if (entry != NULL)
	{
        delete entry;
	}
    if (result != NULL)
    {
        delete result;
    }
    
	return count;
}

bool Directory::exists(const std::string &pathname)
{
	if (pathname.empty())
	{
        return false;
	}
	struct stat info;
	return ::stat(pathname.c_str(), &info) == 0;
}

bool Directory::isAbsolutePath(const std::string &path)
{
    if (path.empty()) 
    {
        return false;
    }

    return path[0] == '/';
}

bool Directory::mkdir(const std::string &dirName)
{
	if (dirName.empty())
	{
        return false;
	}
    return ::mkdir(dirName.c_str(), static_cast<mode_t>(sDefaultMode)) == 0;
}

bool Directory::mkpath(const std::string &pathName)
{
	printf("Directory::mkpath pathName:%s\n", pathName.c_str());

    if (pathName.empty())
	{
        return false;
	}

	if (Directory::exists(pathName))
	{
		return true;
	}

	const std::string parentPath = Directory::parentPath(pathName);
	if (parentPath.empty())
	{
        return ::mkdir(pathName.c_str(), static_cast<mode_t>(sDefaultMode)) == 0;
	}

	if (mkpath(parentPath)) 
	{
        return ::mkdir(pathName.c_str(), static_cast<mode_t>(sDefaultMode)) == 0;
	}

	return false;
}

std::string Directory::parentPath(const std::string &path)
{
	std::string simplePath = Directory::cleanPath(path);
    if (simplePath.empty())
    {
        return "";
    }

    size_t pos = simplePath.find_last_of('/');
    if (string::npos == pos)
    {
        return "";
    }

    if (0 == pos)
    {
        return "/";
    }

    return simplePath.substr(0, pos);
}


}  //~ moon