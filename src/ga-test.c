#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "cfgfile.h"
#include "cfgfile_test.h"
#include "ga-lib.h"
#include "outputcode.h"

int pam_get_item(void *x, int y, void **z);
int pam_set_item(void *x, int y, void **z);
int pam_get_user(const void *pamh, const char **user, const char *prompt);
const char *pam_strerror(void *pamh, int errnum);

/*-----------------------------------------------------------------*/
void
show_result(int status, char *desc)
{
if (status)
	{
	printf("FAIL: %s\n", desc);
	}
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
free(fn);

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

return result;
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

return result;
}

/*-----------------------------------------------------------------*/
int
TEST_gen_verf_code()
{
char key16[] = "A1B2C3D4E5F61234";
char key32[] = "A1B2C3D4E5F61234A1B2C3D4E5F61234";
char key64[] = "A1B2C3D4E5F61234A1B2C3D4E5F61234A1B2C3D4E5F61234A1B2C3D4E5F61234";
int result;
int retval = 0;

result = gen_verf_code(key16, 10000) == 535137 ? 0 : 1;
show_result(result, "gen_verf_code() with 16 bytes");
if (result) retval = result;

result = gen_verf_code(key32, 10000) == 408928 ? 0 : 1;
show_result(result, "gen_verf_code() with 32 bytes");
if (result) retval = result;

result = gen_verf_code(key64, 10000) == 158469 ? 0 : 1;
show_result(result, "gen_verf_code() with 64 bytes");
if (result) retval = result;

return retval;
}

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
	show_result(status, "verf_key(short_key)");
	exit_code = 1;
	}

if ((status = verf_key(long_key)) != 1)
	{
	show_result(status, "verf_key(long_key)");
	exit_code = 1;
	}

if ((status = verf_key(key)) != 0)
	{
	show_result(status, "verf_key(key)");
	exit_code = 1;
	}

return exit_code;
}

/*-----------------------------------------------------------------*/
int
TEST_pam_overrides()
{
int status;
int exit_code = 0;

if ((status = pam_get_item(NULL, 0, NULL)) != 0)
	{
	show_result(status, "pam_get_item()");
	exit_code = 1;
	}

if ((status = pam_set_item(NULL, 0, NULL)) != 0)
	{
	show_result(status, "pam_set_item()");
	exit_code = 1;
	}

if ((status = pam_get_user(NULL, NULL, NULL)) != 0)
	{
	show_result(status, "pam_get_user()");
	exit_code = 1;
	}

if (pam_strerror(NULL, 0) != NULL)
	{
	show_result(1, "pam_strerror()");
	exit_code = 1;
	}

return exit_code;
}

char *
gcf_test(char *basename)
{
char *fn;
char fname[] = "testkeyfile";

fn = malloc(sizeof(fname) + 1);
memset(fn, 0, sizeof(fname) + 1);
strcpy(fn, fname);

return fn;
}

/*-----------------------------------------------------------------*/
int
TEST_output_code_from_args()
{
mode_t saved_umask;
FILE *fp;
char *fn;
char *test_data =
    "bigconglomerate=1234567890123456\n"
    "badkey=\n"
    "=badtag\n"
    "=\n"
    "badline\n"
    "\n"
    "bigcorporation=2345678901234567\n"
    "longentry=00000000000000000000000000000000000000000000000000000000000000000\n"
    "invalidlongentry=6728816425954153529631318083160674393515450726006776061023814462678699164991128381753846105605136468741324878055526633321049426950104566138983963140875035397297293720124852158630158366218315145965159568335318834642993010838793934747981312926591019003238840801133723015618595105368800504912978558460310862813038572245641129711329788850611378016218920806617713884746348092714339472513535743180747866146465448191230991443577290676284182633520772837637211786720096648375527528462426136982076242241276226415409300377743257666641592809034295844713737685784919184509050574670857372022701040664984608029588488810157440074253309115181308937111694779873817866240506077402855317165311065550368936105115827224589897321804457168098094666761597770311249784761637413005512190739467030519847963963866351014035447234503257768453887172858463012389846177509195711664359219045860393273169698339858903438943875705221207502477586952403884671615147776882690555671737856533051362392472590967015280022403849184042837099468979941927980704650460824439\n"
	"badcontent=1234567890123456\n"
	"favesite=abcdefghijklmnop\n"
    "\n"
    ;
char *args[] = { "test", "favesite" };
char obscured_key[] = "0x64,0x67,0x66,0x61,0x60,0x63,0x62,0x6D,0x6C,0x65,0x64,0x67,"
	"0x66,0x61,0x60,0x63,0x00";
int exit_code = 0;
FILE *output = fopen("/dev/null", "w");;

/* Key specified on command line. No compiled key. No config file. */
if (output_code_from_args(2, args, "", output, gcf_test) != EXIT_FAILURE)
		{
	show_result(1, "output_code_from_args(<CLI key, no compiled key, no config file>");
	exit_code = 1;
}

/* No key on command line. Compiled key. No config file. */
if (output_code_from_args(1, args, obscured_key, output, gcf_test) != EXIT_FAILURE)
	{
	show_result(1, "output_code_from_args(<no CLI key, compiled key, no config file>");
	exit_code = 1;
	}

/* No key on command line. No compiled key. No config file. */
if (output_code_from_args(1, args, "", output, gcf_test) != EXIT_FAILURE)
	{
	show_result(1, "output_code_from_args(<no CLI key, no compiled key, no config file>");
	exit_code = 1;
	}

/* Bad file permissions */
saved_umask = umask(007);
fn = gcf_test("");
fp = fopen(fn, "w");
if (fp != NULL) fclose(fp);
if (output_code_from_args(2, args, "", output, gcf_test) != EXIT_FAILURE)
	{
	show_result(1, "output_code_from_args(<bad perms>");
	exit_code = 1;
	}
remove(fn);

/* create test file */
umask(077);
fp = fopen(fn, "w");
if (fp != NULL)
	{
	fputs(test_data, fp);
	fclose(fp);
	}
umask(saved_umask);

/* Retrieval of good key */
args[1] = "favesite";
if (output_code_from_args(2, args, "", output, gcf_test) != EXIT_SUCCESS)
	{
	show_result(1, "output_code_from_args(<favesite>)");
	exit_code = 1;
	}

/* Bad key contents */
args[1] = "badcontent";
if (output_code_from_args(2, args, "", output, gcf_test) != EXIT_FAILURE)
	{
	show_result(1, "output_code_from_args(<badcontent>)");
	exit_code = 1;
	}

/* Bad key length */
args[1] = "invalidlongentry";
if (output_code_from_args(2, args, "", output, gcf_test) != EXIT_FAILURE)
	{
	show_result(1, "output_code_from_args(<CLI key, no compiled key, config file, long entry>");
	exit_code = 1;
	}

/* Bad key content */
/* Not testable */

remove(fn);
free(fn);

return exit_code;
}

/*-----------------------------------------------------------------*/
int
main()
{
int exit_code = EXIT_SUCCESS;

typedef int (*ptr_test_func)(void);

ptr_test_func *test_func;
ptr_test_func test_funcs[] =
	{
	TEST_gen_verf_code,
	TEST_hide_key,
	TEST_reveal_key,
	TEST_get_config_filename,
	TEST_load_key_by_tag,
	TEST_key_verf,
	TEST_pam_overrides,
	TEST_output_code_from_args,
	NULL
	};

test_func = test_funcs;
while (*test_func != NULL)
{
	int ret;

	ret = (*test_func)();
	if (ret)
		{
		exit_code = EXIT_FAILURE;
		}

	test_func++;
}

return exit_code;
}
