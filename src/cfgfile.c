#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "cfgfile.h"

/*-----------------------------------------------------------------*/
int
proc_file(char *tag_request, FILE *fp, char *keybuf, int keybuf_len)
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
	result = CFG_MISC_ERROR;

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
				if (tag_request == NULL
					|| strlen(tag_request) == 0
					|| strcmp(tag, tag_request) == 0)
					{
					strncpy(keybuf, key, keybuf_len);
					result = CFG_KEY_RETRIEVED;
					break;
					}
				}
			}
		}
	}

	return result;
}

/*-----------------------------------------------------------------*/
/* Returns:
 * CFG_KEY_RETRIEVED: key retrieved
 * CFG_MISC_ERROR: misc error
 * CFG_FILE_OPEN_ERROR: file not opened
 * CFG_INVALID_FILE_PERMS: invalid file permissions
 */
int
load_key_by_tag(char *tag_request, char *filename, char *keybuf, int keybuf_len)
{
int result = CFG_KEY_RETRIEVED;
FILE *fp;

if (keybuf == NULL || keybuf_len < 65)
	{
	result = CFG_MISC_ERROR;
	}
else
	{
	*keybuf = '\0';

	fp = fopen(filename, "r");
	if (!fp)
		{
		result = CFG_FILE_OPEN_ERROR;
		}
	else
		{
		result = proc_file(tag_request, fp, keybuf, keybuf_len);
		fclose(fp);
		}
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
