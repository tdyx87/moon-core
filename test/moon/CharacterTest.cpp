#include <moon/Character.h>

#include <gtest/gtest.h>

using namespace moon;

TEST(CharacterTest, isAlnum){
	EXPECT_TRUE(Character::isAlnum('0'));
	EXPECT_TRUE(Character::isAlnum('9'));
	EXPECT_TRUE(Character::isAlnum('A'));
	EXPECT_TRUE(Character::isAlnum('Z'));
	EXPECT_TRUE(Character::isAlnum('a'));
	EXPECT_TRUE(Character::isAlnum('z'));

	EXPECT_FALSE(Character::isAlnum(0));
	EXPECT_FALSE(Character::isAlnum('?'));
}

TEST(CharacterTest, isAlpha){
	EXPECT_TRUE(Character::isAlpha('A'));
	EXPECT_TRUE(Character::isAlpha('Z'));
	EXPECT_TRUE(Character::isAlpha('a'));
	EXPECT_TRUE(Character::isAlpha('z'));

    EXPECT_FALSE(Character::isAlpha('0'));
    EXPECT_FALSE(Character::isAlpha('?'));
}

TEST(CharacterTest, isCntrl){
	EXPECT_TRUE(Character::isCntrl(0));
	EXPECT_TRUE(Character::isCntrl(31));
	EXPECT_TRUE(Character::isCntrl(127));
}

TEST(CharacterTest, isDigit){
	EXPECT_TRUE(Character::isDigit('0'));
	EXPECT_TRUE(Character::isDigit('9'));
	EXPECT_FALSE(Character::isDigit('A'));
	EXPECT_FALSE(Character::isDigit('Z'));
	EXPECT_FALSE(Character::isDigit('a'));
	EXPECT_FALSE(Character::isDigit('z'));
}

TEST(CharacterTest, isGraph){
}

TEST(CharacterTest, isLower){
	EXPECT_TRUE(Character::isLower('a'));
	EXPECT_TRUE(Character::isLower('z'));

	EXPECT_FALSE(Character::isLower('A'));
	EXPECT_FALSE(Character::isLower('Z'));	

    EXPECT_FALSE(Character::isLower('0'));
    EXPECT_FALSE(Character::isLower('?'));
}

TEST(CharacterTest, isUpper){
	EXPECT_TRUE(Character::isUpper('A'));
	EXPECT_TRUE(Character::isUpper('Z'));

	EXPECT_FALSE(Character::isUpper('a'));
	EXPECT_FALSE(Character::isUpper('z'));
    EXPECT_FALSE(Character::isUpper('0'));
    EXPECT_FALSE(Character::isUpper('?'));
}

TEST(CharacterTest, isPrint){
	EXPECT_TRUE(Character::isPrint('A'));
	EXPECT_TRUE(Character::isPrint('Z'));

	EXPECT_TRUE(Character::isPrint('a'));
	EXPECT_TRUE(Character::isPrint('z'));
    EXPECT_TRUE(Character::isPrint('0'));
    EXPECT_TRUE(Character::isPrint('?'));
}

TEST(CharacterTest, isSpace){
	EXPECT_TRUE(Character::isSpace(' '));
	EXPECT_TRUE(Character::isSpace('\t'));
	EXPECT_TRUE(Character::isSpace('\n'));
	EXPECT_TRUE(Character::isSpace('\v'));
	EXPECT_TRUE(Character::isSpace('\f'));
	EXPECT_TRUE(Character::isSpace('\r'));
}

TEST(CharacterTest, isPunct){
	EXPECT_TRUE(Character::isPunct('.'));
	EXPECT_TRUE(Character::isPunct(','));

	EXPECT_FALSE(Character::isPunct('a'));
}

TEST(CharacterTest, isHex){
	EXPECT_TRUE(Character::isHex('0'));
	EXPECT_TRUE(Character::isHex('9'));
	EXPECT_TRUE(Character::isHex('a'));
	EXPECT_TRUE(Character::isHex('f'));
	EXPECT_TRUE(Character::isHex('A'));
	EXPECT_TRUE(Character::isHex('F'));

	EXPECT_FALSE(Character::isHex('g'));
	EXPECT_FALSE(Character::isHex('z'));
	EXPECT_FALSE(Character::isHex('*'));
}

TEST(CharacterTest, toLower){
	EXPECT_EQ(Character::toLower('a'), 'a');
	EXPECT_EQ(Character::toLower('A'), 'a');
	EXPECT_EQ(Character::toLower('z'), 'z');
	EXPECT_EQ(Character::toLower('Z'), 'z');

	EXPECT_EQ(Character::toLower('1'), '1');
	EXPECT_EQ(Character::toLower('*'), '*');
}

TEST(CharacterTest, toUpper){
	EXPECT_EQ(Character::toUpper('a'), 'A');
	EXPECT_EQ(Character::toUpper('A'), 'A');
	EXPECT_EQ(Character::toUpper('z'), 'Z');
	EXPECT_EQ(Character::toUpper('Z'), 'Z');

	EXPECT_EQ(Character::toUpper('1'), '1');
	EXPECT_EQ(Character::toUpper('*'), '*');
}

TEST(CharacterTest, toHex){
	EXPECT_EQ(Character::toHex('0'), 0x00);
	EXPECT_EQ(Character::toHex('9'), 0x09);
	EXPECT_EQ(Character::toHex('A'), 0x0A);
	EXPECT_EQ(Character::toHex('a'), 0x0A);
	EXPECT_EQ(Character::toHex('F'), 0x0F);
	EXPECT_EQ(Character::toHex('f'), 0x0F);

	EXPECT_EQ(Character::toHex('G'), 'G');
	EXPECT_EQ(Character::toHex('*'), '*');
}

TEST(CharacterTest, toChar){
	EXPECT_EQ(Character::toChar(0x00), '0');
	EXPECT_EQ(Character::toChar(0x09), '9');
	EXPECT_EQ(Character::toChar(0x0A), 'A');
	EXPECT_EQ(Character::toChar(0x0F), 'F');

	EXPECT_EQ(Character::toChar('G'), 'G');
	EXPECT_EQ(Character::toChar('*'), '*');
}

TEST(CharacterTest, maxAndMin){
	EXPECT_EQ(Character::MAX_VALUE, 127);
	EXPECT_EQ(Character::MIN_VALUE, -128);
}


