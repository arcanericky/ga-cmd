#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "ga-lib.h"

int main()
{
char key_from_compile[] = { SEED };
char key_from_file[17];
char *key;
int verf_code;

reveal_key(key_from_compile);

key = (load_key(get_config_filename(".ga-cmd"), key_from_file,
	sizeof(key_from_file)) == 0) ? key_from_file : key_from_compile;

verf_code = gen_verf_code(key, time(0) / 30);

printf("%6.6d\n", verf_code);

return EXIT_SUCCESS;
}
