#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "verf.h"

/*-----------------------------------------------------------------*/
int
main(int argc, char *argv[])
{
char *key;
char *p;
char hidden_key[85];

if (argc < 2)
	{
	return EXIT_FAILURE;
	}

key = argv[1];
if (verf_key(key))
	{
	return EXIT_FAILURE;
	}

hide_key(key, hidden_key, sizeof(hidden_key));

printf("%s\n", hidden_key);

return EXIT_SUCCESS;
}
