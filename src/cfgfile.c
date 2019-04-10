#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "cfgfile.h"

/*-----------------------------------------------------------------*/
int
proc_file(FILE *fp, cfg_file_key_handler cfk_service, void *user_data)
{
char linebuf[1024];
int result;
struct stat s;

if (fstat(fileno(fp), &s) || s.st_mode & (S_IRWXO | S_IRWXG))
	{
	result = CFG_INVALID_FILE_PERMS;
	}
else
	{
	result = CFG_KEY_NOT_FOUND;

	while (fgets(linebuf, sizeof(linebuf), fp) != NULL)
		{
		int buflen;
		char *tag;
		char *key;

		/* Trim newline */
		buflen = strlen(linebuf) - 1;
		if (linebuf[buflen] == '\n')
			{
			linebuf[buflen] = '\0';
			}

		/* Skip long lines */
		if (strlen(linebuf) == sizeof(linebuf) - 1)
			{
			char *retcode;

			do
				{
				retcode = fgets(linebuf, sizeof(linebuf), fp);
				} while (strlen(linebuf) == sizeof(linebuf) - 1 && retcode != NULL);
			}
		else
			{
			tag = strtok(linebuf, "=");
			key = strtok(NULL, "\n");

			if (tag != NULL && key != NULL)
				{
				result = cfk_service(tag, key, user_data);

				if (result < 0)
					{
					break;
					}
				}
			}
		}

		result = cfk_service(NULL, NULL, user_data);
	}

return result;
}

/*-----------------------------------------------------------------*/
/* Returns:
 * CFG_KEY_FOUND: key retrieved
 * CFG_KEY_NOT_FOUND: key not found
 * CFG_MISC_ERROR: misc error
 * CFG_FILE_OPEN_ERROR: file not opened
 * CFG_INVALID_FILE_PERMS: invalid file permissions
 */
int
parse_key_file(char *filename, cfg_file_key_handler cfk_service, void *user_data)
{
int result = CFG_KEY_FOUND;
FILE *fp;

fp = fopen(filename, "r");
if (!fp)
	{
	result = CFG_FILE_OPEN_ERROR;
	}
else
	{
	result = proc_file(fp, cfk_service, user_data);
	fclose(fp);
	}

return result;
}

/*-----------------------------------------------------------------*/
char
*get_config_filename(char *file)
{
char *homedir = NULL;
char *result = NULL;
int result_len;

if (file != NULL)
	{
	homedir = getenv("HOME");
	if (homedir != NULL)
		{
		result_len = strlen(file) + strlen(homedir) + 2;
		result = malloc(result_len);

		if (result != NULL)
			{
			strcpy(result, homedir);
			strcat(result, "/");
			strcat(result, file);
			}
		}
	}

return result;
}
