#include <string.h>
#include <stdio.h>

/*-----------------------------------------------------------------*/
int
hide_key(char *key, char *out, int len)
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

/*-----------------------------------------------------------------*/
int
reveal_key(char *key)
{
while (*key)
	{
	*key = *key ^ 0b01010101;
	key++;
	}

return 0;
}

