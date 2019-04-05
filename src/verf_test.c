#include "ga-test.h"
#include "verf.h"
#include "verf_test.h"

/*-----------------------------------------------------------------*/
int
TEST_key_verf()
{
int status;
int exit_code = 0;
char *short_key = "1234";
char *long_key = "1234567890123456789012345678901234567890123456789012345678901234567890";
char *key = "abcdefghijklmnop";

if ((status = verf_key(short_key)) != 1)
	{
	show_test_result(status, "verf_key(short_key)");
	exit_code = 1;
	}

if ((status = verf_key(long_key)) != 1)
	{
	show_test_result(status, "verf_key(long_key)");
	exit_code = 1;
	}

if ((status = verf_key(key)) != 0)
	{
	show_test_result(status, "verf_key(key)");
	exit_code = 1;
	}

return exit_code;
}