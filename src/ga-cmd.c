#include <stdio.h>
#include "cfgfile.h"
#include "outputcode.h"
#include "version.h"

#define VERF_ERROR_PREFIX "Key for %s does not pass verification for "

/*-----------------------------------------------------------------*/
int
main(int argc, char *argv[])
{
#ifndef HMACKEY
#define HMACKEY ""
#endif

#ifndef VERSION
#define VERSION "unknown"
#endif

int exitcode = 0;
char key_from_compile[] = { HMACKEY };

if (!version_option(argc, argv, VERSION, stdout))
    {
    exitcode = output_code_from_args(argc, argv, key_from_compile, stdout, get_config_filename);
    }

return exitcode;
}
