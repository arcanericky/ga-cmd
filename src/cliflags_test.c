#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "cfgfile.h"
#include "cliflags.h"

static int display_version_called = 0;
static int display_list_called = 0;

int
display_version_test(FILE *output, char *version)
{
display_version_called = 1;
return 0;
}

int
display_list_test(FILE *output)
{
display_list_called = 1;
return 0;
}

int TEST_proc_cli_flags()
{
char *version_string = "test-version";
char *arg_zero = "test-cmd";
char *args[] = { arg_zero, "--version" };
FILE *fp;
int actual_result;
int return_code = 0;
char *fn;
mode_t saved_umask;
char *test_data =
	"favesite=abcdefghijklmnop\n"
    "\n"
    ;

FILE *output = fopen("/dev/null", "w");

actual_result = proc_cli_flags(2, args, version_string, output, display_version_test, display_list_test);
if (!actual_result && !display_version_called)
    {
    printf("FAIL: Version should be shown when --version flag is given\n");
    return_code = 1;
    }

optind = 1;
args[1] = "-v";
actual_result = proc_cli_flags(2, args, version_string, output, display_version_test, display_list_test);
if (!actual_result && !display_version_called)
    {
    printf("FAIL: Version should be shown when -v flag is given\n");
    return_code = 1;
    }

optind = 1;
args[1] = "-v";
actual_result = proc_cli_flags(2, args, version_string, output, NULL, NULL);
if (!actual_result)
    {
    printf("FAIL: Default version should be shown when -v flag is given\n");
    return_code = 1;
    }

optind = 1;
args[1] = NULL;
actual_result = proc_cli_flags(1, args, version_string, NULL, display_version_test, display_list_test);
if (actual_result)
    {
    printf("FAIL: Version should not be shown when no flag is given\n");
    return_code = 1;
    }

optind = 1;
args[1] = "--list";
actual_result = proc_cli_flags(2, args, version_string, output, display_version_test, display_list_test);
if (!actual_result && !display_list_called)
    {
    printf("FAIL: Tag list should be shown --list flag is given\n");
    return_code = 1;
    }

optind = 1;
args[1] = "-l";
actual_result = proc_cli_flags(2, args, version_string, output, display_version_test, display_list_test);
if (!actual_result && !display_list_called)
    {
    printf("FAIL: Tag list should be shown -l flag is given\n");
    return_code = 1;
    }

setenv("HOME", ".", 1);
fn = get_config_filename(".ga-cmd");
saved_umask = umask(077);
fp = fopen(fn, "w");
if (fp != NULL)
	{
	fputs(test_data, fp);
	fclose(fp);
	}
umask(saved_umask);

optind = 1;
args[1] = "-l";
actual_result = proc_cli_flags(2, args, version_string, output, NULL, NULL);
if (!actual_result && !display_list_called)
    {
    printf("FAIL: Default tag list should be shown -l flag is given\n");
    return_code = 1;
    }

fclose(output);
remove(fn);
free(fn);

return return_code;
}
