#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "ga-lib.h"

int main()
{
char s[] = { SEED };
int verf_code;

verf_code = gen_verf_code(s, time(0) / 30);

printf("%6.6d\n", verf_code);

return EXIT_SUCCESS;
}
