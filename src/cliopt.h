#ifndef CLIOPT_H_
#define CLIOPT_H_
#define OPT_HELP       (1 << 0)

struct cmdline_options
{
	unsigned int flags;
};
extern struct cmdline_options opt;

int parse_cmdline(int argc, const char *argv[]);

#endif /* CLIOPT_H_ */
