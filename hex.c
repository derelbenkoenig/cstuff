#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

static void print_usage(char *executable);
static void encode(void);
static void decode(void);

int main(int argc, char *argv[])
{
	char *arg;
	if (argc > 2) {
		print_usage(argv[0]);
		exit(1);
	}
	arg = argv[1];
	if (argc == 1 || 0 == strncmp(arg, "-e", 2)) {
		return encode();
	} else if (0 == strncmp(arg, "-d", 2)) {
		return decode();
	} else {
		print_usage(argv[0]);
		exit(1);
	}
	/* should not be reachable? */
	return 1;
}

static void print_usage(char *executable)
{
	dprintf(2, "Usage: %s [-d|-e]\n", executable);
}


