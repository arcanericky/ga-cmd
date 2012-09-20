#include <stdio.h>
#include <time.h>

int compute_code(char *, int, int);
int base32_decode(char *, char *, int);

int pam_get_item(void *x, int y, void **z) { return 0; }
int pam_set_item(void *x, int y, void **z) { return 0; }

int main()
{
char s[] = { SEED };
char *p = s;

while (*p)
{
	*p = *p ^ 0b01010101;
	p++;
}

char x[11];
printf("%6.6d\n", compute_code(x, base32_decode(s, x, 16), time(0) / 30));

return 0;
}
