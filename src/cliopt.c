#include "cliopt.h"

struct cmdline_options opt;

void printHelp(void)
{
	puts("Usage: hardview [-h]");
	puts("");
	puts("Program options:");
	puts("    -h    print help");
}

int parse_cmdline(int argc, const char *argv[]) 
{
	opt.flags |= OPT_HELP;
	return 0;
}
