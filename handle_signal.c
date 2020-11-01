#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <errno.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#undef NDEBUG

static void setup_sighdlr(void);
static void setup_files(void);
static void handle_sig(int signum, siginfo_t *info, void *ucontext);

int main(int argc, char **argv)
{
	unsigned int ret;
	struct timespec sleeptime, remtime;
	int *foo = (int *)42;

	// setup_files();
	setup_sighdlr();

	for (;;) {
		sleeptime.tv_sec = 10;
		sleeptime.tv_nsec = 0;

		ret = nanosleep(&sleeptime, &remtime);
		if (ret == -1) {
			if (errno == EINTR) {
				dprintf(2, "interrupted with %f seconds remaining\n"
						, remtime.tv_sec + ((double)remtime.tv_nsec) / 1000000000.0);
			} else {
				perror("error in nanosleep()");
			}
		}

		// *foo = 54;

	}
}

static void setup_sighdlr(void)
{
	struct sigaction act;

	act.sa_sigaction = handle_sig;
	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_SIGINFO;

	sigaction(SIGINT, &act, NULL);
	sigaction(SIGTERM, &act, NULL);
	sigaction(SIGQUIT, &act, NULL);
	sigaction(SIGUSR1, &act, NULL);
	sigaction(SIGUSR2, &act, NULL);
	sigaction(SIGHUP, &act, NULL);
	sigaction(SIGPIPE, &act, NULL);
	sigaction(SIGSEGV, &act, NULL);
	sigaction(SIGKILL, &act, NULL);
}

static void handle_sig(int signum, siginfo_t *info, void *ucontext)
{
	int fd, errfd;

	switch (signum) {
		case SIGINT:
		case SIGTERM:
			psiginfo(info, "Received signal");
			exit(0);
			break;

		case SIGKILL:
			psiginfo(info, "Got SIGKILL, IDK how though. should be impossible");
			exit(1);
			break;

		case SIGSEGV:
			fd = open("segfault.log", O_RDWR | O_CREAT | O_APPEND, 0666);
			if (fd < 0) {
				perror("couldn't open segfault.log");
				exit(1);
			}

			errfd = dup2(fd, STDERR_FILENO);
			if (errfd != 2) {
				perror("couldn't make stderr point to segfault.log");
				exit(1);
			}

			psiginfo(info, "Received segfault");
			abort();
			break;

		default:
			psiginfo(info, "Received signal");
			break;
	}
}

static void setup_files()
{
	int fd;

	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	fd = open("foo.in", O_RDWR | O_CREAT, 0666);
	assert(fd == 0);
	fd = open("foo.out", O_RDWR | O_CREAT | O_APPEND, 0666);
	assert(fd == 1);
	fd = open("foo.err", O_RDWR | O_CREAT | O_APPEND, 0666);
	assert(fd == 2);
}
