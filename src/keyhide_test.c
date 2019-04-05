#include <stdio.h>
#include <string.h>

#include "ga-test.h"
#include "keyhide.h"
#include "keyhide_test.h"

/*-----------------------------------------------------------------*/
int
TEST_hide_and_reveal_key()
{
int result;
char test_key[] = "1234567890123456";
char hidden_key[sizeof(test_key) * 5];

char test_key_long[] = "ABCDEFGHIJLMNOPQRSTUVWXYZ234567";
char hidden_key_long[sizeof(hidden_key) * 5];

char *hkld_key_stepper, *hidden_key_stepper;
int key_byte;
char hidden_key_long_decoded[sizeof(test_key_long)];

/* Test for computing bytes needed */
result = hide_key(test_key, NULL, 0) == sizeof(test_key) * 5 ? 0 : 1;
show_test_result(result, "hide_key(\"1234567890123456\", NULL, 0)");

/* Test all valid key characters */
hide_key(test_key_long, hidden_key_long, sizeof(hidden_key_long));

hkld_key_stepper = hidden_key_long_decoded;
hidden_key_stepper = hidden_key_long;
do
	{
	sscanf(hidden_key_stepper, "%x", &key_byte);
	*hkld_key_stepper++ = key_byte;
	hidden_key_stepper += 5;
	} while (key_byte != 0);

reveal_key(hidden_key_long_decoded);

if (strcmp(test_key_long, hidden_key_long_decoded) != 0)
	{
	result = 1;
	show_test_result(result, "hide_key()/reveal_key() all valid characters");
	}

return result;
}

