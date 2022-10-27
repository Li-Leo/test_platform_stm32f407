/************************************************
* DESCRIPTION:
*   you can custom the function define at this file to
*		let it work right
*
************************************************/
#include "ssz_check.h"
#include "assert.h"
#include "stdio.h"


#ifdef DEBUG
//call it when assert fail by ssz_assert, need hold or terminate system
void ssz_on_assert_fail(const char* filename, int lineno, const char* expression)
{
	printf("%s:%d %s -- Assert Fail\n", filename, lineno, expression);
	assert(0);//lint !e506
}
//call it when check fail by ssz_check, need hold or terminate system
void ssz_on_check_fail(const char* filename, int lineno, const char* expression)
{
	printf("%s:%d %s -- Check Fail\n", filename, lineno, expression);
	assert(0);//lint !e506
	while (1);
}
#else
//call it when check fail by ssz_check, need hold or terminate system
void ssz_on_check_fail()
{
	printf("check fail!\n");
	assert(0);//lint !e506
	while (1);
}
#endif


