#include <getopt.h>
#include <stdio.h>

int version_option(int argc, char *argv[], char *version, FILE *fp)
{
int f_version = 0;
int option_index = 0;
int return_code = 0;
struct option long_options[] =
{
    { "version", no_argument, &f_version, 1 },
    { NULL, 0, NULL, 0 }
};

if (getopt_long(argc, argv, "-:v", long_options, &option_index) == 'v' || f_version == 1)
    {
    fprintf(fp, "ga-cmd version %s\n", version);
    return_code = 1;
    }

return return_code;
}