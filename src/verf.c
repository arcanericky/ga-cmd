#include <string.h>

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

return 0;
}

/*-----------------------------------------------------------------*/
int
verf_key(char *key)
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
