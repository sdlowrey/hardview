#include "cliopt.h"

#include <stdlib.h>

struct cmdline_options opt;

void printHelp(void)
{
	puts("Usage: hardview [-h]");
	puts("");
	puts("Program options:");
	puts("    -h    print help");
}

int parse_cmdline(int argc, char * const argv[]) 
{
	int option;
	const char *optstr = "h";
	static struct option longopts[] = {
		{"help", no_argument, NULL, 'h'},
		{0,0,0,0}
	};

	while ((option = getopt_long(argc, argv, optstr, longopts, NULL)) != -1)
	{
		switch (option) {
		case 'h':
			opt.flags |= OPT_HELP;
			break;
		}
	}
	return 0;
}
