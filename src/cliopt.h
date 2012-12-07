#ifndef CLIOPT_H_
#define CLIOPT_H_

#include <getopt.h>

#define OPT_HELP       (1 << 0)

struct cmdline_options
{
	unsigned int flags;
};
extern struct cmdline_options opt;

int parse_cmdline(int argc, char * const argv[]);

#endif /* CLIOPT_H_ */
