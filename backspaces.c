#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#define ARRAY_SIZE 21

int main(int argc, char *argv[])
{
	ssize_t write_res;

	char backspaces[ARRAY_SIZE] = {
		'\b', '\b', '\b', '\b', '\b',
		'\b', '\b', '\b', '\b', '\b',
		'\b', '\b', '\b', '\b', '\b',
		'\b', '\b', '\b', '\b', '\b',
		0,
	};

	errno = 0;
	write_res = write(STDOUT_FILENO, backspaces, ARRAY_SIZE);

	if (write_res != ARRAY_SIZE) {
		fprintf(stderr, "tried to write %d bytes, return value was %d (error %d %s)"
				, ARRAY_SIZE
				, write_res
				, errno
				, strerror(errno));
		exit(1);
	}

	return 0;
}

