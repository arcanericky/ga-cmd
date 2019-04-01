#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "cfgfile.h"
#include "codegen.h"
#include "keyhide.h"
#include "outputcode.h"
#include "verf.h"

#define VERF_ERROR_PREFIX "Key for %s does not pass verification for "

/*-----------------------------------------------------------------*/
int
output_code_from_args(int argc, char *argv[], char *key_from_compile, FILE *output, char *(*gcf)(char *))
{
// 64 characters + null terminator
char key_from_file[65];
char *key;
int verf_code;
int retval;
int exitcode;
char *config_filename;

reveal_key(key_from_compile);

if (argc == 2)
	{
	config_filename = gcf(".ga-cmd");
	retval = load_key_by_tag(argv[1], config_filename, key_from_file, sizeof(key_from_file));

	if (retval == CFG_KEY_RETRIEVED)
		{
		key = key_from_file;
		}
	else
		{
		switch (retval)
			{
			case CFG_MISC_ERROR:
				fprintf(output, "Could not find key for %s in %s.\n", argv[1], config_filename);
				exitcode = EXIT_FAILURE;
			break;
			case CFG_FILE_OPEN_ERROR:
				fprintf(output, "Configuration file %s could not be opened.\n", config_filename);
				exitcode = EXIT_FAILURE;
			break;
			case CFG_INVALID_FILE_PERMS:
				fprintf(output, "Configuration file %s has insecure file permissions.\n"
					"  Only the owner should have read/write permissions.\n", config_filename);
				exitcode = EXIT_FAILURE;
			break;
			}

		key = NULL;
		}

	free(config_filename);
	}
else if (strlen(key_from_compile) == 0)
	{
	fprintf(output, "No key compiled interally and no key specified on command line or found.\n");
	key = NULL;
	}
else
	{
	key = key_from_compile;
	}

if (key != NULL)
	{
	switch (verf_key(key))
		{
		case VERF_OK:
			verf_code = gen_verf_code(key, time(0) / 30);
			fprintf(output, "%6.6d\n", verf_code);

			exitcode = EXIT_SUCCESS;
		break;
		case VERF_BAD_LEN:
			fprintf(output, VERF_ERROR_PREFIX "length.\n", argv[1]);
			exitcode = EXIT_FAILURE;
		break;
		case VERF_BAD_CONTENT:
			fprintf(output, VERF_ERROR_PREFIX "content.\n", argv[1]);
			exitcode = EXIT_FAILURE;
		break;
		}
	}

return exitcode;
}
