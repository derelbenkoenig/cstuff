#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define INBUF_SIZE 256
#define OUTBUF_SIZE (2 * INBUF_SIZE)

static unsigned char inbuf[INBUF_SIZE];
static unsigned char outbuf[OUTBUF_SIZE];

int main(int argc, char *argv[])
{
	ssize_t read_res, write_res, bytes_to_write;
	unsigned char c;

	for (;;) {
		bytes_to_write = 0;
		read_res = read(0, inbuf, INBUF_SIZE);
		if (read_res < 0) goto err;
		if (read_res == 0) goto end;
		for (int i = 0; i < read_res; i++) {
			c = inbuf[i];
			if (c != '\n') {
				outbuf[2 * i] = c;
				outbuf[1 + 2 * i] = '\n';
				bytes_to_write += 2;
			}
		}
		do {
			write_res = write(1, outbuf, bytes_to_write);
			if (write_res == -1) goto err;
			bytes_to_write -= write_res;
		} while (bytes_to_write > 0);
	}
end:
	return 0;
err:
	return 1;
}
