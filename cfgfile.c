#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

/*-----------------------------------------------------------------*/
int
load_key(char *filename, char *keybuf, int keybuf_len)
{
int result = 0;
FILE *fp;
struct stat s;

if (filename == NULL || keybuf == NULL)
	{
	return 1;
	}

fp = fopen(filename, "r");
if (fp != NULL)
	{
	if (fstat(fileno(fp), &s))
		{
		result = 1;
		}
	else
		{
		if (s.st_mode & (S_IRWXO | S_IRWXG))
			{
			result = 1;
			}
		else
			{
			int bytes_read;
			memset(keybuf, 0, keybuf_len);
			bytes_read = fread(keybuf, sizeof(char), keybuf_len - 1, fp);
			fclose(fp);
			if (bytes_read != 16)
				{
				result = 1;
				}
			}
		}
	}
else
	{
	result = 1;
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
