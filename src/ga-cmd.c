#include <stdio.h>
#include "cfgfile.h"
#include "outputcode.h"
#include "cliflags.h"

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

int exit_code = 0;
char key_from_compile[] = { HMACKEY };

if (!proc_cli_flags(argc, argv, VERSION, stdout, NULL, NULL))
    {
    exit_code = output_code_from_args(argc, argv, key_from_compile, stdout, NULL, NULL, NULL);
    }

return exit_code;
}
