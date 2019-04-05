#include <stddef.h>

#include "codegen.h"
#include "codegen_test.h"
#include "ga-test.h"

int pam_get_item(void *x, int y, void **z);
int pam_set_item(void *x, int y, void **z);
int pam_get_user(const void *pamh, const char **user, const char *prompt);
const char *pam_strerror(void *pamh, int errnum);

/*-----------------------------------------------------------------*/
int
TEST_gen_verf_code()
{
char key16[] = "A1B2C3D4E5F61234";
char key32[] = "A1B2C3D4E5F61234A1B2C3D4E5F61234";
char key64[] = "A1B2C3D4E5F61234A1B2C3D4E5F61234A1B2C3D4E5F61234A1B2C3D4E5F61234";
int result;
int retval = 0;

result = gen_verf_code(key16, 10000) == 535137 ? 0 : 1;
show_test_result(result, "gen_verf_code() with 16 bytes");
if (result) retval = result;

result = gen_verf_code(key32, 10000) == 408928 ? 0 : 1;
show_test_result(result, "gen_verf_code() with 32 bytes");
if (result) retval = result;

result = gen_verf_code(key64, 10000) == 158469 ? 0 : 1;
show_test_result(result, "gen_verf_code() with 64 bytes");
if (result) retval = result;

return retval;
}

/*-----------------------------------------------------------------*/
int
TEST_pam_overrides()
{
int status;
int exit_code = 0;

if ((status = pam_get_item(NULL, 0, NULL)) != 0)
	{
	show_test_result(status, "pam_get_item()");
	exit_code = 1;
	}

if ((status = pam_set_item(NULL, 0, NULL)) != 0)
	{
	show_test_result(status, "pam_set_item()");
	exit_code = 1;
	}

if ((status = pam_get_user(NULL, NULL, NULL)) != 0)
	{
	show_test_result(status, "pam_get_user()");
	exit_code = 1;
	}

if (pam_strerror(NULL, 0) != NULL)
	{
	show_test_result(1, "pam_strerror()");
	exit_code = 1;
	}

return exit_code;
}
