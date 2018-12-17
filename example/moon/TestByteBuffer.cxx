#include <moon/ByteBuffer.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

using namespace std;
using namespace moon;

#define INT_EQUAL(actual, expected)                               \
    if ( (actual) != expected) {                                  \
        printf("actual[%zu] expected[%d]\n", (actual), expected); \
        return ;                                                  \
    }

static void test1()
{
    ByteBuffer buffer;
    buffer.debugPrint();

    buffer.append("Hello");
    printf("getAllAsString:%s\n", buffer.getAllAsString().c_str());
    buffer.debugPrint();

    string str1 = buffer.getString(2);
    printf("str1:%s\n", str1.c_str());
    printf("peek:%s#\n", buffer.data());
}

static void test2()
{
	ByteBuffer buffer;

	int8_t i8 = 98;
	int16_t i16 = 9876;
	int32_t i32 = 98765432;
	int64_t i64 = 987654321012;
	long long1 = 98765432101234;

	INT_EQUAL(buffer.appendInt8(i8), 1);
	INT_EQUAL(buffer.appendInt16(i16), 2);
	INT_EQUAL(buffer.appendInt32(i32), 4);
	INT_EQUAL(buffer.appendInt64(i64), 8);
	INT_EQUAL(buffer.appendLong(long1), static_cast<int>(sizeof(long)));

	buffer.debugPrint();
	printf("%d\n", buffer.getInt8());
	printf("%d\n", buffer.getInt16());
	printf("%d\n", buffer.getInt32());
	printf("%ld\n", buffer.getInt64());
	printf("%ld\n", buffer.getLong());
	buffer.debugPrint();
}

int main()
{
    test2();

    printf("------main end------\n");
	return 0;
}