#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#include "cfgfile.h"
#include "cliflags.h"

typedef struct
{
FILE *output;
} user_data_list;

int
cfg_file_list_key_service(char *tag, char *key, void *user_data)
{
if (tag != NULL)
    {
    fprintf(((user_data_list *) user_data)->output, "%s\n", tag);
    }

return 0;
}

int
display_version(FILE *output, char *version)
{
fprintf(output, "ga-cmd version %s\n", version);

return 0;
}

int
display_list(FILE *output)
{
char *config_filename;
user_data_list udl;

config_filename = get_config_filename(".ga-cmd");
udl.output = output;
parse_key_file(config_filename, cfg_file_list_key_service, &udl);

free(config_filename);

return 0;
}

int
proc_cli_flags(int argc, char *argv[], char *version, FILE *output, version_handler version_service, list_handler list_service)
{
int f_version = 0;
int f_list = 0;
int gol;
int option_index = 0;
int return_code = 0;
struct option long_options[] =
{
    { "version", no_argument, &f_version, 1 },
    { "list", no_argument, &f_list, 1 },
    { NULL, 0, NULL, 0 }
};

if (output == NULL)
    {
    output = stdout;
    }

if (version_service == NULL)
    {
    version_service = display_version;
    }

if (list_service == NULL)
    {
    list_service = display_list;
    }

gol = getopt_long(argc, argv, "-:vl", long_options, &option_index);
switch (gol)
{
    case 'l':
        f_list = 1;
        break;
    case 'v':
        f_version = 1;
        break;
}

if (f_version)
    {
    version_service(output, version);
    return_code = 1;
    }

if (f_list)
    {
    list_service(output);
    return_code = 1;
    }

return return_code;
}