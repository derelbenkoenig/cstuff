#define _POSIX_C_SOURCE 200809L

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <limits.h>

#define ERRBUF_LEN (PATH_MAX + 64)

static void fix_file(int fd, char *filename);
static void handle_err(int error, char *format, ...);

int main(int argc, char *argv[])
{
	for (int i = 1; i < argc; i++) {
		char *filename = argv[i];
		int fd = open(filename, O_RDWR);

		if (fd == -1) {
			handle_err(errno, "cannot open file %s", filename);
			continue;
		}

		fix_file(fd, filename);
		close(fd);
	}

	return 0;
}

static void handle_err(int error, char *format, ...)
{
#define SEPARATOR ": "
#define SEPARATOR_LEN 2
	int res;
	size_t format_len, err_len;
	va_list format_args;
	va_start(format_args, format);
	char errbuf[ERRBUF_LEN], formatbuf[ERRBUF_LEN];

	errbuf[ERRBUF_LEN - 1] = '\0';
	formatbuf[ERRBUF_LEN - 1] = '\0';

	strncpy(formatbuf, format, ERRBUF_LEN - 1);
	format_len = strlen(formatbuf);

	if (format_len + SEPARATOR_LEN + 1 > ERRBUF_LEN) {
		goto panic;
	}

	strncat(formatbuf, SEPARATOR, ERRBUF_LEN - 1 - format_len);

	res = strerror_r(error, errbuf, ERRBUF_LEN - 1);

	if (res != 0) {
		goto panic;
	}

	err_len = strlen(errbuf);

	if (err_len + format_len + 1 > ERRBUF_LEN) {
		goto panic;
	}

	strncat(formatbuf, errbuf, ERRBUF_LEN - format_len - 1);

	if (formatbuf[ERRBUF_LEN - 1] != '\0') {
		goto panic;
	}
	
	vdprintf(2, formatbuf, format_args);
	va_end(format_args);

	return;

panic:
	dprintf(2, "can't transcribe error to string. Maybe a filepath is way too long\n");
	abort();
}

static void fix_file(int fd, char *filename)
{
	off_t seek_res;
	ssize_t res;
	char character;
	/* TODO seek to end of file, if last character is not \n, write \n to end of file */

	seek_res = lseek(fd, -1, SEEK_END);
	if (seek_res == -1) {
		handle_err(errno, "cannot seek to end of file %s", filename);
		return;
	}

	res = read(fd, &character, 1);
	if (res == -1) {
		handle_err(errno, "cannot read from file %s", filename);
		return;
	}

	if (character == '\n') {
                return;
	}

	res = write(fd, "\n", 1);
	if (res == -1) {
		handle_err(errno, "cannot write to file %s", filename);
	}
}
