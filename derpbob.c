#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <ctype.h>

#define BUF_SIZE 128
#define CHAR_DIFF ((unsigned char) 'a' - 'A')

int main(int argc, char *argv[])
{
	unsigned char buf[BUF_SIZE];
	int bytes_read, bytes_written, flag = 0;

	while (1) {
		bytes_read = read(0, buf, BUF_SIZE);
		if (-1 == bytes_read) {
			perror("error reading stdin");
			exit(1);
		}

		if (0 == bytes_read) {
			break;
		}

		for (int i = 0; i < bytes_read; i++) {
			unsigned char *c = buf + i;
			if (isalpha(*c)) {
				if (flag) {
					*c += isupper(*c) ? CHAR_DIFF : -CHAR_DIFF;
				}
				flag ^= 1;
			}
		}

		do {
			bytes_written = write(1, buf, bytes_read);
			if (bytes_written == -1) {
				perror("error writing to stdout");
				exit(1);
			}
			bytes_read -= bytes_written;
		} while (bytes_read > 0);
	}
}
