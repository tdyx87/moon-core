#include <moon/Character.h>
#include <limits>

using namespace moon;

const char Character::MAX_VALUE = std::numeric_limits<char>::max();  // char能表示的最大值
const char Character::MIN_VALUE = std::numeric_limits<char>::min();  // char能表示的最小值

unsigned char Character::toHex(unsigned char c)
{
	if( ('0' <= c) && (c <= '9'))
	{
        return c - 0x30;
	}
	else if( ('A' <= c) && (c <= 'F'))
	{
        return c - 'A' + 10;
	}
	else if( ('a' <= c) && (c <= 'f'))
	{
        return c - 'a' + 10;
	}

	return c;
}

unsigned char Character::toChar(unsigned char c)
{
	if (c < 0x0A)
	{
		return c + '0';
	}
	else if (c <= 0x0F)
	{
		return c + 'A' - 10;
	}

	return c;
}

