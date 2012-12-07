#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include "../src/cliopt.h"

const char *PROG_NAME = "hardview";

START_TEST (test_parse_invalid_option)
{
	const int argc = 2;
	char *argv[2] = {PROG_NAME, "-z"};

	int e = parse_cmdline(argc, argv);

	fail_unless(e == EXIT_FAILURE, "parse_cmdline() did not fail on invalid option");
}
END_TEST

START_TEST (test_parse_help)
{
	const int argc = 2;
	char *argv[2] = {PROG_NAME, "-h"};

	int e = parse_cmdline(argc, argv);

	fail_unless(e == 0, "parse_cmdline() returned %e for -h option");
	fail_unless(opt.flags & OPT_HELP, "option flags incorrect for -h");
}
END_TEST

Suite *opt_suite(void)
{
	Suite *s = suite_create("cli");
	TCase *tc_cli = tcase_create("options");
	tcase_add_test(tc_cli, test_parse_invalid_option);
	tcase_add_test(tc_cli, test_parse_help);
	suite_add_tcase(s, tc_cli);
	return s;
}
	
int main(void) 
{
	int numfailed;
	Suite *s = opt_suite();
	SRunner *sr = srunner_create(s);
	srunner_run_all(sr, CK_NORMAL);
	numfailed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return(numfailed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
