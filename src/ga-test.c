#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cfgfile.h"

/*-----------------------------------------------------------------*/
void
show_result(int status, char *desc)
{
printf("%s: %s\n", status ? "FAIL" : "PASS", desc);
}

/*-----------------------------------------------------------------*/
int
TEST_get_config_filename()
{
char *fn;
int result = 1;

fn = get_config_filename("test");
result = fn == NULL;
show_result(result, "get_config_filename()");

return result;
}

/*-----------------------------------------------------------------*/
int
TEST_load_key()
{
char *test_key = "7777777777777777";
char *key_filename = "keyfile";
char key[17];
int result;
FILE *fp;
mode_t saved_umask;

/* No file */
result = load_key(key_filename, key, sizeof(key));
show_result(!result, "load_key(<file does not exist>)");

/* Bad group permissions */
saved_umask = umask(007);
fp = fopen(key_filename, "w");
if (fp != NULL) fclose(fp);
result = load_key(key_filename, key, sizeof(key));
remove(key_filename);
show_result(!result, "load_key(<bad group perms>)");

/* Bad world permissions */
umask(070);
fp = fopen(key_filename, "w");
if (fp != NULL) fclose(fp);
result = load_key(key_filename, key, sizeof(key));
remove(key_filename);
show_result(!result, "load_key(<bad world perms>)");

/* No data */
umask(077);
fp = fopen(key_filename, "w");
if (fp != NULL) fclose(fp);
result = load_key(key_filename, key, sizeof(key));
remove(key_filename);
show_result(!result, "load_key(<no data in file>)");

/* Good file and key */

fp = fopen(key_filename, "w");
if (fp != NULL)
	{
	fputs(test_key, fp);
	fclose(fp);
	}

result = load_key(key_filename, key, sizeof(key));
remove(key_filename);

if (result == 0)
	{
	result = strcmp(key, test_key);
	}

show_result(result, "load_key(\"keyfile\", \"1234567890123456\")");

umask(saved_umask);

return result;
}

/*-----------------------------------------------------------------*/
int
TEST_reveal_key()
{
char key[] = { 0x64,0x67,0x66,0x61,0x60,0x63,0x62,0x6D,0x6C,0x65,0x64,0x67,
	0x66,0x61,0x60,0x63,0x00 };
reveal_key(key);
int result = strcmp(key, "1234567890123456");
show_result(result, "reveal_key(char [])");

return 0;
}

/*-----------------------------------------------------------------*/
int
TEST_hide_key()
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

/*-----------------------------------------------------------------*/
int
TEST_gen_verf_code()
{
char key[] = "A1B2C3D4E5F61234";
int result;

result = gen_verf_code(key, 10000) == 535137 ? 0 : 1;
show_result(result, "gen_verf_code()");

return (0);
}

/*-----------------------------------------------------------------*/
int
main()
{
typedef int (*ptr_test_func)(void);

ptr_test_func *test_func;
ptr_test_func test_funcs[] =
	{
	TEST_gen_verf_code,
	TEST_hide_key,
	TEST_reveal_key,
	TEST_load_key,
	TEST_get_config_filename,
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
