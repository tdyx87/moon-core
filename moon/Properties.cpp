# include <moon/Properties.h>
# include <moon/Double.h>
# include <moon/Integer.h>
# include <moon/Long.h>

#include <fstream>

namespace moon
{

static std::string trimSpace(const std::string& src)
{
	std::string whiteSpaces(" \t\f\v\n\r");

	size_t begin = src.find_first_not_of(whiteSpaces);
	size_t end = src.find_last_not_of(whiteSpaces);
	if (begin == end)
	{
		if (std::string::npos == begin)
		{
			return "";
		}
		return src;
	}

	return src.substr(begin, end - begin + 1);
}

std::string Properties::getProperty(const std::string& name, const std::string defaultValue) const
{
	if (mProperties.count(name) == 1) 
	{
		return std::string(mProperties.at(name));
	}

	return defaultValue;
}

int Properties::getInt(const std::string& name, int defaultValue) const
{
    if (mProperties.count(name) == 0) 
    {
        return defaultValue;
    }

    return Integer::parseInt(mProperties.at(name), defaultValue);
}

int Properties::getLong(const std::string& name, long defaultValue) const
{
    if (mProperties.count(name) == 0)
    {
        return defaultValue;
    }

    return Long::parseLong(mProperties.at(name), defaultValue);
}

int Properties::getDouble(const std::string& name, double defaultValue) const
{
    if (mProperties.count(name) == 0) 
    {
        return defaultValue;
    }

    return Double::parseDouble(mProperties.at(name), defaultValue);
}

int Properties::load(const std::string& fileName) 
{
	std::ifstream in(fileName.c_str());
	if (!in) 
	{
		return __LINE__;		
	}

	while (in) 
	{
		char buf[1024] = {0};

		in.getline(buf, sizeof(buf));

		std::string line = trimSpace(buf);
		if ( (line.empty()) || (line[0] == '#') )
		{
			continue;
		}
		parseLine(line);
	}

	return true;
}

void Properties::parseLine(const std::string &line) 
{
    size_t index = line.find('=');
	if (std::string::npos == index) 
	{
		// TODO: should print some warning logs
		return ;
	}

	if (index == 0) 
	{
		// TODO: should print some warning logs
        return ;
	}

	std::string key = line.substr(0, index);
	std::string value = line.substr(index + 1);
    mProperties[key] = value;
}

}  //~ moon