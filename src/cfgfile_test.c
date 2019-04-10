#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "cfgfile.h"
#include "ga-test.h"

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
    "favesite=6543210987654321\n"
    "\n"
    ;

typedef struct
{
char *requested_tag;
char found_key[65];
} cfgfile_callback_data_test;

int
cfg_file_key_service_test(char *tag, char *key, void *user_data)
{
int result = 0;
static int lookup_status = CFG_KEY_NOT_FOUND;

if (tag == NULL && key == NULL)
    {
    /* EOF / Final Call */
    result = lookup_status;
    lookup_status = CFG_KEY_NOT_FOUND;
    }
else
    {
    if (((cfgfile_callback_data_test *) user_data)->requested_tag == NULL
       || strlen(((cfgfile_callback_data_test *) user_data)->requested_tag) == 0
       || strcmp(tag, ((cfgfile_callback_data_test *) user_data)->requested_tag) == 0)
       {
      lookup_status = CFG_KEY_FOUND;
      strncpy(((cfgfile_callback_data_test *) user_data)->found_key, key, sizeof(((cfgfile_callback_data_test *) user_data)->found_key));

      /* Caller exit read loop */
      result = -1;
      }
    }

return result;
}

void
show_fail_result(char *display_text)
{
    printf("FAIL: %s\n", display_text);
}


/*-----------------------------------------------------------------*/
int
TEST_get_config_filename()
{
char *fn;
int result = 1;

fn = get_config_filename("test");
result = fn == NULL;
show_test_result(result, "get_config_filename()");
free(fn);

return result;
}

int TEST_parse_key_file()
{
char *key_filename = "testkeyfile";
char *expected_key;
FILE *fp;
int expected_result;
int actual_result;
int exit_code = 0;
mode_t saved_umask;
char output[1024];
cfgfile_callback_data_test ccd;

remove(key_filename);

/* No file */
expected_result = CFG_FILE_OPEN_ERROR;
actual_result = parse_key_file(key_filename, cfg_file_key_service_test, &ccd);
if (expected_result != actual_result)
    {
    snprintf(output, sizeof(output),
        "parse_key_file(<file does not exist>). Expected: %d. Actual: %d.",
        expected_result, actual_result);
    show_fail_result(output);
    exit_code = 1;
    }

/* Bad group permissions */
saved_umask = umask(007);
fp = fopen(key_filename, "w");
if (fp != NULL) fclose(fp);
expected_result = CFG_INVALID_FILE_PERMS;
actual_result = parse_key_file(key_filename, cfg_file_key_service_test, &ccd);
remove(key_filename);
if (expected_result != actual_result)
    {
    snprintf(output, sizeof(output),
        "parse_key_file(<bad group perms>). Expected: %d. Actual: %d.",
        expected_result, actual_result);
    show_fail_result(output);
    exit_code = 1;
    }

/* Bad world permissions */
umask(070);
fp = fopen(key_filename, "w");
if (fp != NULL) fclose(fp);
expected_result = CFG_INVALID_FILE_PERMS;
actual_result = parse_key_file(key_filename, cfg_file_key_service_test, &ccd);
remove(key_filename);
if (expected_result != actual_result)
    {
    snprintf(output, sizeof(output),
        "parse_key_file(<bad world perms>). Expected: %d. Actual: %d.",
        expected_result, actual_result);
    show_fail_result(output);
    exit_code = 1;
    }

/* No data */
umask(077);
fp = fopen(key_filename, "w");
if (fp != NULL) fclose(fp);
expected_result = CFG_KEY_NOT_FOUND;
actual_result = parse_key_file(key_filename, cfg_file_key_service_test, &ccd);
remove(key_filename);
if (expected_result != actual_result)
    {
    snprintf(output, sizeof(output),
        "parse_key_file(<Empty config file>). Expected: %d. Actual: %d.",
        expected_result, actual_result);
    show_fail_result(output);
    exit_code = 1;
    }

/* create test file */
fp = fopen(key_filename, "w");
if (fp != NULL)
	{
	fputs(test_data, fp);
	fclose(fp);
	}
umask(saved_umask);

/* load default on empty string (first line in data file) */
ccd.requested_tag = "";
expected_key = "1234567890123456";
actual_result = parse_key_file(key_filename, cfg_file_key_service_test, &ccd);
if (strncmp(ccd.found_key, expected_key, sizeof(ccd.found_key)) != 0)
    {
    snprintf(output, sizeof(output),
        "Default Empty Key. Expected: %s, Actual: %s\n",
        expected_key, ccd.found_key);
    show_fail_result(output);
    exit_code = 1;
    }

/* load default on NULL string (first line in data file) */
expected_key = "1234567890123456";
actual_result = parse_key_file(key_filename, cfg_file_key_service_test, &ccd);
if (strncmp(ccd.found_key, expected_key, sizeof(ccd.found_key)) != 0)
    {
    snprintf(output, sizeof(output),
        "Default NULL Key. Expected: %s, Actual: %s\n",
        expected_key, ccd.found_key);
    show_fail_result(output);
    exit_code = 1;
    }

/* key exists */
ccd.requested_tag = "bigcorporation";
expected_result = CFG_KEY_FOUND;
expected_key = "2345678901234567";
actual_result = parse_key_file(key_filename, cfg_file_key_service_test, &ccd);
if (strncmp(ccd.found_key, expected_key, sizeof(ccd.found_key)) != 0)
    {
    snprintf(output, sizeof(output),
        "Load %s Key. Expected: %s, Actual: %s\n", ccd.requested_tag, expected_key, ccd.found_key);
    show_fail_result(output);
    exit_code = 1;
    }

/* Key does not exit*/
ccd.requested_tag = "keydoesnotexist";
expected_result = CFG_KEY_NOT_FOUND;
expected_key = "";
actual_result = parse_key_file(key_filename, cfg_file_key_service_test, &ccd);
if (expected_result != actual_result)
    {
    snprintf(output, sizeof(output),
        "Load %s Key. Expected: %d, Actual: %d\n", ccd.requested_tag, expected_result, actual_result);
    show_fail_result(output);
    exit_code = 1;
    }

/* Last entry */
ccd.requested_tag = "favesite";
expected_key = "6543210987654321";
actual_result = parse_key_file(key_filename, cfg_file_key_service_test, &ccd);
if (strncmp(ccd.found_key, expected_key, sizeof(ccd.found_key)) != 0)
    {
    snprintf(output, sizeof(output),
        "Load %s Key. Expected: %s, Actual: %s\n", ccd.requested_tag, expected_key, ccd.found_key);
    show_fail_result(output);
    exit_code = 1;
    }

/* 64 byte key */
ccd.requested_tag = "longentry";
expected_key = "00000000000000000000000000000000000000000000000000000000000000000";
actual_result = parse_key_file(key_filename, cfg_file_key_service_test, &ccd);
if (strncmp(ccd.found_key, expected_key, sizeof(ccd.found_key)) != 0)
    {
    snprintf(output, sizeof(output),
        "Load %s Key. Expected: %s, Actual: %s\n", ccd.requested_tag, expected_key, ccd.found_key);
    show_fail_result(output);
    exit_code = 1;
    }

remove(key_filename);

return exit_code;
}

/*-----------------------------------------------------------------*/
int
TEST_proc_file()
{
int exit_code = 0;
int retval;
FILE *fp;

fp = fopen("/dev/null", "r");
if (fp == NULL)
    {
    exit_code = 1;
    }
else
{
    fp->_fileno = -1;
    retval = proc_file(fp, NULL, 0);
    if (retval != CFG_INVALID_FILE_PERMS)
        {
        show_fail_result("failed fstat() in proc_file should return CFG_INVALID_FILE_PERMS");
        exit_code = 1;
        }

    fclose(fp);
}

return exit_code;
}
