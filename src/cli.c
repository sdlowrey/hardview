#include "cliopt.h"

int main(int argc, char * const argv[])
{
	if (parse_cmdline(argc, argv) != 0) {
		printHelp();
		return 1;
	}

	if (opt.flags & OPT_HELP) {
		printHelp();
	}


}
