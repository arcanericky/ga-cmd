#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ga-lib.h"

void show_result(int status, char *desc)
{
printf("%s: %s\n", status ? "FAIL" : "PASS", desc);
}

int TEST_reveal_key()
{
char key[] = { 0x64,0x67,0x66,0x61,0x60,0x63,0x62,0x6D,0x6C,0x65,0x64,0x67,
	0x66,0x61,0x60,0x63,0x00 };
reveal_key(key);
int result = strcmp(key, "1234567890123456");
show_result(result, "reveal_key(char [])");

return 0;
}

int TEST_hide_key()
{
int result;
char hidden_key[85];

/* Test for computing bytes needed */
result = hide_key("1234567890123456", NULL, 0) == 85 ? 0 : 1;
show_result(result, "hide_key(\"1234567890123456\", NULL, 0)");

/* Test for generating obfuscated key */
hide_key("1234567890123456", hidden_key, sizeof(hidden_key));
result = strcmp(hidden_key,
	"0x64,0x67,0x66,0x61,0x60,0x63,0x62,0x6D,0x6C,0x65,0x64,0x67,"
	"0x66,0x61,0x60,0x63,0x00");
show_result(result, "hide_key(\"1234567890123456\", hidden_key, 85)");

return 0;
}

int TEST_gen_verf_code()
{
char key[] = "1234567890123456";
int result;

result = gen_verf_code(key, 10000) == 126865 ? 0 : 1;
show_result(result, "gen_verf_code(\"1234567890123456\", 10000)");

return (0);
}

int main()
{
typedef int (*ptr_test_func)(void);

ptr_test_func *test_func;
ptr_test_func test_funcs[] =
	{
	TEST_gen_verf_code,
	TEST_hide_key,
	TEST_reveal_key,
	NULL
	};

test_func = test_funcs;
while (*test_func != NULL)
{
	int ret;

	ret = (*test_func)();
	if (ret)
		{
		return EXIT_FAILURE;
		}

	test_func++;
}

return EXIT_SUCCESS;
}
