#include <stdio.h>
#include <stdlib.h>

#include <gtest/gtest.h>

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc,argv);     
	int ret = RUN_ALL_TESTS();
    if(ret == 0) 
	{
		printf("Gtest success\n");
        return 0;
	}
    
	//abort();
	return -1;
}