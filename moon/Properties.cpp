# include <moon/Properties.h>
# include <moon/Logger.h>
# include <moon/Number.h>

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

    return Number::parseInt(mProperties.at(name), defaultValue);
}

long Properties::getLong(const std::string& name, long defaultValue) const
{
    if (mProperties.count(name) == 0)
    {
        return defaultValue;
    }

    return Number::parseLong(mProperties.at(name), defaultValue);
}

double Properties::getDouble(const std::string& name, double defaultValue) const
{
    if (mProperties.count(name) == 0) 
    {
        return defaultValue;
    }

    return Number::parseDouble(mProperties.at(name), defaultValue);
}

bool Properties::load(const std::string& fileName) 
{
	std::ifstream in(fileName.c_str());
	if (!in) 
	{
		return false;		
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
	if (std::string::npos == index) {
		LOGGER_WARN("invalid line:%s", line.c_str());
		return ;
	}

	if (index == 0) {
		LOGGER_WARN("invalid line:%s", line.c_str());
        return ;
	}

	std::string key = trimSpace(line.substr(0, index));
	if (key.empty()) {
        LOGGER_WARN("invalid line:%s", line.c_str());
		return ;
	}
	std::string value = line.substr(index + 1);
    mProperties[key] = trimSpace(value);
    LOGGER_DEBUG("key[%s] = %s", key.c_str(), value.c_str());
}


}  //~ moon