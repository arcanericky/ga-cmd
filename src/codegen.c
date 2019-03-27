#include <string.h>

int compute_code(char *, int, int);
int base32_decode(char *, char *, int);

int pam_get_item(void *x, int y, void **z) { return 0; }
int pam_set_item(void *x, int y, void **z) { return 0; }
int pam_get_user(const void *pamh, const char **user, const char *prompt) { return 0; };
const char *pam_strerror(void *pamh, int errnum) { return 0; };

/*-----------------------------------------------------------------*/
int
gen_verf_code(char *key, int timeval)
{
int key_parm;
int verf_code;
char x[65];

key_parm = base32_decode(key, x, strlen(key));
verf_code = compute_code(x, key_parm, timeval);

return verf_code;
}
