#include "cliopt.h"

int main(int argc, const char **argv)
{
	if (parse_cmdline(argc, argv) !=0) {
		printHelp();
		return 1;
	}

	if ((opt.flags |= OPT_HELP) == 1) {
		printHelp();
	}


}
