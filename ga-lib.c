#include <string.h>
#include <time.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include "ga-lib.h"

int compute_code(char *, int, int);
int base32_decode(char *, char *, int);

int pam_get_item(void *x, int y, void **z) { return 0; }
int pam_set_item(void *x, int y, void **z) { return 0; }

int verf_key_len(char *key)
{
if (strlen(key) != 16)
	{
	return 1;
	}

return 0;
}

int verf_key_chars(char *key)
{
/* TODO: Validate all characters */

return 0;
}

int verf_key(char *key)
{
if (verf_key_len(key))
	{
	return 1;
	}

if (verf_key_chars(key))
	{
	return 2;
	}

return 0;
}

int hide_key(char *key, char *out, int len)
{
/* TODO: Use len to prevent buffer overrun */

char byte[6];
int req_len = 0;

if (out)
	{
	*out = 0;
	}

while (*key)
{
	if (out)
		{
		sprintf(byte, "0x%2.2X,", *key ^ 0b01010101);
		strcat(out, byte);
		}
	req_len += 5;
	key++;
}

if (out)
	{
	strcat(out, "0x00");
	}

req_len += 5;

return req_len;
}

int reveal_key(char *key)
{
while (*key)
	{
	*key = *key ^ 0b01010101;
	key++;
	}

return 0;
}

int gen_verf_code(char *key, int timeval)
{
int key_parm;
int verf_code;
char x[11];

key_parm = base32_decode(key, x, 16);
verf_code = compute_code(x, key_parm, timeval);

return verf_code;
}

int load_key(char *filename, char *keybuf, int keybuf_len)
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

char * get_config_filename(char *file)
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
