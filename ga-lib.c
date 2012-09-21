#include <string.h>
#include <time.h>
#include <stdio.h>
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

reveal_key(key);

key_parm = base32_decode(key, x, 16);
verf_code = compute_code(x, key_parm, timeval);

return verf_code;
}
