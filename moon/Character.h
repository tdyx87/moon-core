/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/
#ifndef MOON_CHARACTER_H_
#define MOON_CHARACTER_H_

#include <ctype.h>

namespace moon
{

/**
  The Character class offers some help operations about char.
*/
class Character
{
public:
	// Returns true if c is an alphanumeric character(0~9 or a~z or A~Z).
	static inline bool isAlnum(int c){return ::isalnum(c) != 0 ? true : false;}

	// Returns true if c is a letter(a~z or A~Z).
	static inline bool isAlpha(int c){return ::isalpha(c) != 0 ? true : false;}

	// Returns true if c is a control character.
	static inline bool isCntrl(int c){return ::iscntrl(c) != 0 ? true : false;}

	// Returns ture if c is a decimal-digit-character(0~9).
	static inline bool isDigit(int c){return ::isdigit(c) != 0 ? true : false;}

	// Returns true if c is a graphical character.
	static inline bool isGraph(int c){return ::isgraph(c) != 0 ? true : false;}

	// Returns true if c is a lowercase letter(a~z).
	static inline bool isLower(int c){return ::islower(c) != 0 ? true : false;}

	// Returns true if c is a uppercase letter(A~Z).
	static inline bool isUpper(int c){return ::isupper(c) != 0 ? true : false;}

	// Returns true if c is a printable character. A printable character 
	// is a non-control character.
	static inline bool isPrint(int c){return ::isprint(c) != 0 ? true : false;}
	
	// Returns true if c is a white-space character(' ', '\t', '\n', '\v', '\f', '\r').
	static inline bool isSpace(int c){return ::isspace(c) != 0 ? true : false;}

	// Returns true if c is a punctuation character.
	static inline bool isPunct(int c){return ::ispunct(c) != 0 ? true : false;}

	// Returns true if c is a hexadecimal character(0~9 or a~f or A~F).
	static inline bool isHex(int c){return ::isxdigit(c) != 0 ? true : false;}
 
    // Returns lowercase equivalent to c, if such value exists, or returns c unchanged.
	static inline int toLower(int c){return ::tolower(c);}

	// Returns uppercase equivalent to c, if such value exists, or returns c unchanged.
	static inline int toUpper(int c){return ::toupper(c);}

	/**
	  如果字符c是0~9, a~f, A~F之间的字符，返回'c'字符所表示的十六进制值。
	  例如：toHex('0')=0x00, toHex('F')=0x0F; toHex('Z')='Z'
	  否则原样返回c
	*/
	static int toHex(int c);
	
	/**
	  如果c的值介于[0, 0x0F]之间,返回字符c对应的字符值。
	  例如：toChar(0x00)='0', toChar(0x0F)='F'.
	  否则原样返回c
	*/
	static int toChar(int c);
public:
	static const char MAX_VALUE;  // char能表示的最大值
	static const char MIN_VALUE;  // char能表示的最小值

};  //~ Character

}  //~ moon

#endif //~ 



