#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>

#define FILENAME1 "file1"
#define FILENAME2 "file2"

#define PHRASE "writing to file after its other file descriptor was closed"

int main(int argc, char **argv)
{
	int fd_a, fd_b, fd_c, fd_d, flags;
	flags = O_RDWR | O_CREAT | O_TRUNC;

	fd_a = open(FILENAME1, flags, 0666);
	fd_b = dup(fd_a);

	fd_c = open(FILENAME2, flags, 0666);
	fd_d = dup2(fd_c, fd_b);

	if (strlen(PHRASE) != write(fd_a, PHRASE, strlen(PHRASE))) {
		exit(1);
	}
	return 0;
}
