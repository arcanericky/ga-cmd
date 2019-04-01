#include <string.h>

#include "verf.h"

int base32_decode(char *, char *, int);

/*-----------------------------------------------------------------*/
int
verf_key_len(char *key)
{
int keylen;

keylen = strlen(key);

if (keylen < 16 || keylen > 64)
	{
	return 1;
	}

return 0;
}

/*-----------------------------------------------------------------*/
int
verf_key_chars(char *key)
{
/* TODO: Validate all characters */
int retval = 0;
char x[65];

if (base32_decode(key, x, strlen(key)) < 0)
	{
	retval = 1;
	};

return retval;
}

/*-----------------------------------------------------------------*/
int
verf_key(char *key)
{
if (verf_key_len(key))
	{
	return VERF_BAD_LEN;
	}

if (verf_key_chars(key))
	{
	return VERF_BAD_CONTENT;
	}

return VERF_OK;
}
