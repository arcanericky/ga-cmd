#include <getopt.h>
#include <stdio.h>
#include "version.h"

int TEST_version_option()
{
char *version_string = "test-version";
char *arg_zero = "test-cmd";
char *args[] = { arg_zero, "--version" };
FILE *fp;
int actual_result;
int return_code = 0;

fp = fopen("/dev/null", "w");

actual_result = version_option(2, args, version_string, fp);
if (!actual_result)
    {
    printf("FAIL: Version should be shown when --version flag is given\n");
    return_code = 1;
    }

optind = 1;
args[1] = "-v";
actual_result = version_option(2, args, version_string, fp);
if (!actual_result)
    {
    printf("FAIL: Version should be shown when -v flag is given\n");
    return_code = 1;
    }

optind = 1;
args[1] = NULL;
actual_result = version_option(1, args, version_string, fp);
if (actual_result)
    {
    printf("FAIL: Version should not be shown when no flag is given\n");
    return_code = 1;
    }

fclose (fp);

return return_code;
}
