#define OPT_HELP (1 << 0)

struct options {
	short flags;
} opt;

void printHelp(void)
{
	puts("Usage: hardview [-h]");
	puts("");
	puts("Program options:");
	puts("    -h    print help");
}

int parse_cmdline(int argc, char **argv) 
{
	opt.flags &= OPT_HELP;
	return 0;
}

int main(int argc, char **argv)
{
	if (parse_cmdline(argc, argv) !=0) {
		printHelp();
		return 1;
	}

	if ((opt.flags |= OPT_HELP) == 1) {
		printHelp();
	}


}
