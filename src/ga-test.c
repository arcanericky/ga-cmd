#include <stdio.h>
#include <stdlib.h>

#include "cfgfile_test.h"
#include "codegen_test.h"
#include "keyhide_test.h"
#include "outputcode_test.h"
#include "verf_test.h"
#include "version_test.h"

/*-----------------------------------------------------------------*/
void
show_test_result(int status, char *desc)
{
if (status)
	{
	printf("FAIL: %s\n", desc);
	}
}

/*-----------------------------------------------------------------*/
int
main()
{
int exit_code = EXIT_SUCCESS;
typedef int (*ptr_test_func)(void);
struct test_routine {
	ptr_test_func test_func;
	char *description;
};
struct test_routine *test_routine_stepper;
struct test_routine test_routines[] = {
	{ TEST_gen_verf_code, "TEST_gen_verf_code" },
	{ TEST_hide_and_reveal_key, "TEST_hide_and_reveal_key" },
	{ TEST_get_config_filename, "TEST_get_config_filename" },
	{ TEST_load_key_by_tag, "TEST_load_key_by_tag" },
	{ TEST_key_verf, "TEST_key_verf" },
	{ TEST_pam_overrides, "TEST_pam_overrides" },
	{ TEST_output_code_from_args, "TEST_output_code_from_args" },
	{ TEST_version_option, "TEST_version_option" },
	{ NULL, NULL }
};

test_routine_stepper = test_routines;
while ((*test_routine_stepper).test_func != NULL)
	{
	int ret;
	char *text_result;

	ret = (*test_routine_stepper).test_func();
	if (ret)
		{
		exit_code = EXIT_FAILURE;
		text_result = "FAILED";
		}
	else
		{
		text_result = "PASSED";
		}

	printf("%s: %s\n", text_result, (*test_routine_stepper).description);

	test_routine_stepper++;
	}

return exit_code;
}
