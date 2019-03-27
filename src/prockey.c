#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "verf.h"
#include "ga-lib.h"

/*-----------------------------------------------------------------*/
int
main(int argc, char *argv[])
{
char *key;
char *p;
// 64 characters + null terminator transforms to 4 character + ","
//   representation of each character. Last character a null instead
//   of a ","
char hidden_key[(65*5)];

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
