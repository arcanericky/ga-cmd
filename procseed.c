#include <string.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
char *p;

if (strlen(argv[1]) != 16)
{
	return 1;
}

p = argv[1];
while (*p)
{
	printf("%d,", *p ^ 0b01010101);
	p++;
}
printf("0");

return 0;
}
