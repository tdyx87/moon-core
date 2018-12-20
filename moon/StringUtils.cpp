#include <moon/StringUtils.h>
#include <moon/Character.h>

#include <assert.h>
#include <string.h>

namespace moon
{
namespace StringUtils
{

/**
  将s指向的字符串offset字节后的count字节转换成大小或者小写。
  如果offset大于等于字符串s的长度，则不对字符串s做任何改动。
  如果count大于字符串s可转换的字节数，则只转换可转换的字节。
  @params toLower true表示转换为小写，false表示转换为大写 
*/
char* toLowerOrUpperCase(bool toLower, char *s, size_t count, size_t offset)
{
	if (NULL == s) {
		return s;
	}

	const size_t sLen = strlen(s);  
	if (offset >= sLen) {
		return s;
	}

	size_t endPos = offset + count;
	if ( (0 == count) || (endPos > sLen) ) {
		endPos = sLen;
	}
	const char *end = s + endPos;

	if (toLower) {
		for (char *p = s + offset; p != end; ++p) {
			*p = static_cast<char>(Character::toLower(*p));
		}
	} else {
		for (char *p = s+offset; p != end; ++p) {
			*p = static_cast<char>(Character::toUpper(*p));
		}
	}

	return s;
}

string& toLowerOrUpperCase(bool toLower, string& s, size_t count, size_t offset)
{
	toLowerOrUpperCase(toLower, &*(s.begin()), count, offset);
	return s;
}

char* toLowerCase(char* s, size_t count, size_t offset)
{
	return toLowerOrUpperCase(true, s, count, offset);
}

string& toLowerCase(string& s, size_t count, size_t offset)
{
	return toLowerOrUpperCase(true, s, count, offset);
}

char* toUpperCase(char* s, size_t count, size_t offset)
{
	return toLowerOrUpperCase(false, s, count, offset);
}

string& toUpperCase(string& s, size_t count, size_t offset)
{
	return toLowerOrUpperCase(false, s, count, offset);
}

// 异常字符串所有的空格
string removeAllSpace(const std::string& src)
{
	string destStr;
	const size_t n = src.length();

	for (size_t i = 0; i < n; ++i)
	{
		if (!Character::isSpace(src[i]))
		{
			destStr += src[i];
		}
	}

	return destStr;
}

string trimSpace(const std::string& src, int mode)
{
	const string whiteSpaces(" \t\f\v\n\r");
	size_t begin = 0;
	size_t end = string::npos;

	if (kTrimLeft & mode) {
        begin = src.find_first_not_of(whiteSpaces);
	}
	if (kTrimRight & mode) {
		end = src.find_last_not_of(whiteSpaces);
	}

	if ( (string::npos != begin) && (string::npos != end) ) {
        return src.substr(begin, end - begin + 1);
	} 
	else if (string::npos == begin) {
        return src.substr(0, end + 1);
	}
	return src.substr(begin);
}

}  // ~StringUtils
}  // ~moon
