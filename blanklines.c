#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

static void usage()
{
	dprintf(2, "Usage: blanklines <n>\nPrints n blank lines to stdout.");
}

int main(int argc, char *argv[])
{
	if (argc < 2) {
		usage();
		exit(1);
	}

	long num_lines = strtol(argv[1], NULL, 10);
	char *buf = malloc(num_lines * sizeof(char));

	for (int i = 0; i < num_lines; i++) {
		buf[i] = '\n';
	}

	long written = write(1, buf, num_lines * sizeof(char));
	if (written != num_lines) {
		dprintf(2
			, "Error, tried to write %d blank lines, got %d instead.\n%s"
			, num_lines
			, written
			, written < 0 ? strerror(errno) : "");
		exit(1);
	}

	return 0;
}
