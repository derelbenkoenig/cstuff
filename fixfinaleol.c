#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>

static int fix_file(int fd);

int main(int argc, char *argv[])
{
	for (int i = 1; i < argc; i++) {
		char *filename = argv[i];
		int fd = open(filename, O_RDWR);
		if (fd == -1) {
			perror("could not open() file");
			continue;
		}
		
	}
}

static int fix_file(int fd)
{
	/* TODO seek to end of file, if last character is not \n, write \n to end of file */
}
