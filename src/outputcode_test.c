#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "ga-test.h"
#include "cfgfile.h"
#include "outputcode.h"
#include "outputcode_test.h"

char *
gcf_test(char *basename)
{
char *fn;
char fname[] = ".ga-cmd";

fn = malloc(sizeof(fname) + 1);
memset(fn, 0, sizeof(fname) + 1);
strcpy(fn, fname);

return fn;
}

int
pkf_test(char *filename, cfg_file_key_handler service, void *user_data)
{
return -1;
}

int
vk_test(char *key)
{
return -1;
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
FILE *output = fopen("/dev/null", "w");

/* Key specified on command line. No compiled key. No config file. */
if (output_code_from_args(2, args, "", output, gcf_test, NULL, NULL) != EXIT_FAILURE)
		{
	show_test_result(1, "output_code_from_args(<CLI key, no compiled key, no config file>");
	exit_code = 1;
}

/* No key on command line. Compiled key. No config file. */
if (output_code_from_args(1, args, obscured_key, output, gcf_test, NULL, NULL) != EXIT_FAILURE)
	{
	show_test_result(1, "output_code_from_args(<no CLI key, compiled key, no config file>");
	exit_code = 1;
	}

/* No key on command line. No compiled key. No config file. */
if (output_code_from_args(1, args, "", output, gcf_test, NULL, NULL) != EXIT_FAILURE)
	{
	show_test_result(1, "output_code_from_args(<no CLI key, no compiled key, no config file>");
	exit_code = 1;
	}

/* Bad file permissions */
saved_umask = umask(007);
fn = gcf_test(".ga-cmd" /* arg not used */ );
fp = fopen(fn, "w");
if (fp != NULL) fclose(fp);
if (output_code_from_args(2, args, "", output, gcf_test, NULL, NULL) != EXIT_FAILURE)
	{
	show_test_result(1, "output_code_from_args(<bad perms>");
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
if (output_code_from_args(2, args, "", output, gcf_test, NULL, NULL) != EXIT_SUCCESS)
	{
	show_test_result(1, "output_code_from_args(<favesite>)");
	exit_code = 1;
	}

/* Bad key contents */
args[1] = "badcontent";
if (output_code_from_args(2, args, "", output, gcf_test, NULL, NULL) != EXIT_FAILURE)
	{
	show_test_result(1, "output_code_from_args(<badcontent>)");
	exit_code = 1;
	}

/* Bad key length */
args[1] = "invalidlongentry";
if (output_code_from_args(2, args, "", output, gcf_test, NULL, NULL) != EXIT_FAILURE)
	{
	show_test_result(1, "output_code_from_args(<CLI key, no compiled key, config file, long entry>");
	exit_code = 1;
	}

/* Bad key content */
/* Not testable */

/* load_key_by_tag() returns invalid error code */
if (output_code_from_args(2, args, "", output, NULL, pkf_test, NULL) != EXIT_FAILURE)
{
	show_test_result(1, "output_code_from_args should return EXIT_FAILURE on invalid error code from load_key_by_tag");
	exit_code = 1;
}

/* verf_key() returns invalid error code */
args[1] = "badcontent";
if (output_code_from_args(2, args, "", output, NULL, NULL, vk_test) != EXIT_FAILURE)
{
	show_test_result(1, "output_code_from_args should return EXIT_FAILURE on invalid error code from verf_key");
	exit_code = 1;
}

fclose(output);
remove(fn);
free(fn);

return exit_code;
}
