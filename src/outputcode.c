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

int
output_code(FILE *output, char *tag, char *key)
{
int verf_code;
int exit_code;

switch (verf_key(key))
	{
	case VERF_OK:
		verf_code = gen_verf_code(key, time(0) / 30);
		fprintf(output, "%6.6d\n", verf_code);
		exit_code = EXIT_SUCCESS;
	break;
	case VERF_BAD_LEN:
		fprintf(output, VERF_ERROR_PREFIX "length.\n", tag);
		exit_code = EXIT_FAILURE;
	break;
	case VERF_BAD_CONTENT:
		fprintf(output, VERF_ERROR_PREFIX "content.\n", tag);
		exit_code = EXIT_FAILURE;
	break;
	}

return exit_code;
}

typedef struct
{
char *requested_tag;
char found_key[65];
int callback_exit_code;
FILE *output_fp;
} cfgfile_callback_data;

int
cfg_file_key_service(char *tag, char *key, void *user_data)
{
int result = 0;
static int lookup_status = CFG_KEY_NOT_FOUND;
cfgfile_callback_data *ccd;

if (tag == NULL && key == NULL)
    {
    /* EOF / Final Call */
    result = lookup_status;
	lookup_status = CFG_KEY_NOT_FOUND;
    }
else
    {
	ccd = (cfgfile_callback_data *) user_data;
    if (ccd->requested_tag == NULL
       || strlen(ccd->requested_tag) == 0
       || strcmp(tag, ccd->requested_tag) == 0)
       {
      lookup_status = CFG_KEY_FOUND;
      strncpy(ccd->found_key, key, sizeof(ccd->found_key));

	  ccd->callback_exit_code = output_code(ccd->output_fp, tag, key);
      /* Caller exit read loop */
      result = -1;
      }
    }

return result;
}

/*-----------------------------------------------------------------*/
int
output_code_from_args(int argc, char *argv[], char *key_from_compile, FILE *output, get_config_filename_handler gcf, parse_key_file_handler pkfh, verf_key_handler vk)
{
/* 64 characters + null terminator */
char key_from_file[65];
char *key;
int verf_code;
int retval;
int exit_code;
char *config_filename;
cfgfile_callback_data ccd;

if (gcf == NULL)
	{
	gcf = get_config_filename;
	}

if (pkfh == NULL)
	{
	pkfh = parse_key_file;
	}

if (vk == NULL)
	{
	vk = verf_key;
	}

reveal_key(key_from_compile);

if (argc == 2)
	{
	config_filename = gcf(".ga-cmd");

	ccd.requested_tag = argv[1];
	ccd.output_fp = output;
	retval = pkfh(config_filename, cfg_file_key_service, &ccd);
	exit_code = ccd.callback_exit_code;

	switch (retval)
		{
		case CFG_KEY_FOUND:
			key = ccd.found_key;
			break;
		case CFG_KEY_NOT_FOUND:
			fprintf(output, "Could not find key for %s in %s.\n", argv[1], config_filename);
			key = NULL;
			exit_code = EXIT_FAILURE;			
			break;
		case CFG_FILE_OPEN_ERROR:
			fprintf(output, "Configuration file %s could not be opened.\n", config_filename);
			key = NULL;
			exit_code = EXIT_FAILURE;
			break;
		case CFG_INVALID_FILE_PERMS:
			fprintf(output, "Configuration file %s has insecure file permissions.\n"
				"  Only the owner should have read/write permissions.\n", config_filename);
			key = NULL;	
			exit_code = EXIT_FAILURE;
			break;
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
	exit_code = output_code(output, ccd.requested_tag, key_from_compile);
	}

return exit_code;
}
