#include <stdio.h>
#include "cfgfile.h"
#include "outputcode.h"

#define VERF_ERROR_PREFIX "Key for %s does not pass verification for "

/*-----------------------------------------------------------------*/
int
main(int argc, char *argv[])
{
#ifndef HMACKEY
#define HMACKEY ""
#endif

int exitcode;
char key_from_compile[] = { HMACKEY };

exitcode = output_code_from_args(argc, argv, key_from_compile, stdout, get_config_filename);

return exitcode;
}
