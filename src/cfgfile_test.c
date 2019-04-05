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

int TEST_load_key_by_tag()
{
char *key_filename = "testkeyfile";
char *expected_key;
char *requested_key;
FILE *fp;
int expected_result;
int actual_result;
int exit_code = 0;
mode_t saved_umask;
char output[1024];
char keybuf[1024];

remove(key_filename);

/* Invalid buffer */
expected_result = CFG_MISC_ERROR;
actual_result = load_key_by_tag("", key_filename, NULL, sizeof(keybuf));
if (expected_result != actual_result)
    {
    snprintf(output, sizeof(output),
        "load_key_by_tag(<NULL buffer>). Expected: %d. Actual: %d.",
        expected_result, actual_result);
    show_fail_result(output);
    exit_code = 1;
    }

/* Short buffer */
expected_result = CFG_MISC_ERROR;
actual_result = load_key_by_tag("", key_filename, keybuf, 64);
if (expected_result != actual_result)
    {
    snprintf(output, sizeof(output),
        "load_key_by_tag(<short buffer>). Expected: %d. Actual: %d.",
        expected_result, actual_result);
    show_fail_result(output);
    exit_code = 1;
    }

/* No file */
expected_result = CFG_FILE_OPEN_ERROR;
actual_result = load_key_by_tag("", key_filename, keybuf, sizeof(keybuf));
if (expected_result != actual_result)
    {
    snprintf(output, sizeof(output),
        "load_key_by_tag(<file does not exist>). Expected: %d. Actual: %d.",
        expected_result, actual_result);
    show_fail_result(output);
    exit_code = 1;
    }

/* Bad group permissions */
saved_umask = umask(007);
fp = fopen(key_filename, "w");
if (fp != NULL) fclose(fp);
expected_result = CFG_INVALID_FILE_PERMS;
actual_result = load_key_by_tag("", key_filename, keybuf, sizeof(keybuf));
remove(key_filename);
if (expected_result != actual_result)
    {
    snprintf(output, sizeof(output),
        "load_key_by_tag(<bad group perms>). Expected: %d. Actual: %d.",
        expected_result, actual_result);
    show_fail_result(output);
    exit_code = 1;
    }

/* Bad world permissions */
umask(070);
fp = fopen(key_filename, "w");
if (fp != NULL) fclose(fp);
expected_result = CFG_INVALID_FILE_PERMS;
actual_result = load_key_by_tag("", key_filename, keybuf, sizeof(keybuf));
remove(key_filename);
if (expected_result != actual_result)
    {
    snprintf(output, sizeof(output),
        "load_key_by_tag(<bad world perms>). Expected: %d. Actual: %d.",
        expected_result, actual_result);
    show_fail_result(output);
    exit_code = 1;
    }

/* No data */
umask(077);
fp = fopen(key_filename, "w");
if (fp != NULL) fclose(fp);
expected_result = CFG_MISC_ERROR;
actual_result = load_key_by_tag("", key_filename, keybuf, sizeof(keybuf));
remove(key_filename);
if (expected_result != actual_result)
    {
    snprintf(output, sizeof(output),
        "load_key_by_tag(<bad world perms>). Expected: %d. Actual: %d.",
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
expected_key = "1234567890123456";
actual_result = load_key_by_tag("", key_filename, keybuf, sizeof(keybuf));
if (strncmp(keybuf, expected_key, sizeof(keybuf)) != 0)
    {
    snprintf(output, sizeof(output),
        "Default Empty Key. Expected: %s, Actual: %s\n",
        expected_key, keybuf);
    show_fail_result(output);
    exit_code = 1;
    }

/* load default on NULL string (first line in data file) */
expected_key = "1234567890123456";
actual_result = load_key_by_tag(NULL, key_filename, keybuf, sizeof(keybuf));
if (strncmp(keybuf, expected_key, sizeof(keybuf)) != 0)
    {
    snprintf(output, sizeof(output),
        "Default NULL Key. Expected: %s, Actual: %s\n",
        expected_key, keybuf);
    show_fail_result(output);
    exit_code = 1;
    }

/* key exists */
requested_key = "bigcorporation";
expected_result = CFG_KEY_RETRIEVED;
expected_key = "2345678901234567";
actual_result = load_key_by_tag(requested_key, key_filename, keybuf, sizeof(keybuf));
if (strncmp(keybuf, expected_key, sizeof(keybuf)) != 0)
    {
    snprintf(output, sizeof(output),
        "Load %s Key. Expected: %s, Actual: %s\n", requested_key, expected_key, keybuf);
    show_fail_result(output);
    exit_code = 1;
    }

/* Key does not exit*/
requested_key = "keydoesnotexist";
expected_result = CFG_MISC_ERROR;
expected_key = "";
actual_result = load_key_by_tag(requested_key, key_filename, keybuf, sizeof(keybuf));
if (expected_result != actual_result)
    {
    snprintf(output, sizeof(output),
        "Load %s Key. Expected: %d, Actual: %d\n", requested_key, expected_result, actual_result);
    show_fail_result(output);
    exit_code = 1;
    }

if (strncmp(keybuf, expected_key, sizeof(keybuf)) != 0)
    {
    snprintf(output, sizeof(output),
        "Load %s Key. Expected: %s, Actual: %s\n", requested_key, expected_key, keybuf);
    show_fail_result(output);
    exit_code = 1;
    }

/* Last entry */
requested_key = "favesite";
expected_key = "6543210987654321";
actual_result = load_key_by_tag(requested_key, key_filename, keybuf, sizeof(keybuf));
if (strncmp(keybuf, expected_key, sizeof(keybuf)) != 0)
    {
    snprintf(output, sizeof(output),
        "Load %s Key. Expected: %s, Actual: %s\n", requested_key, expected_key, keybuf);
    show_fail_result(output);
    exit_code = 1;
    }

/* 64 byte key */
requested_key = "longentry";
expected_key = "00000000000000000000000000000000000000000000000000000000000000000";
actual_result = load_key_by_tag(requested_key, key_filename, keybuf, sizeof(keybuf));
if (strncmp(keybuf, expected_key, sizeof(keybuf)) != 0)
    {
    snprintf(output, sizeof(output),
        "Load %s Key. Expected: %s, Actual: %s\n", requested_key, expected_key, keybuf);
    show_fail_result(output);
    exit_code = 1;
    }

remove(key_filename);

return exit_code;
}

// int
// main()
// {
// return TEST_load_key_by_tag();
// }