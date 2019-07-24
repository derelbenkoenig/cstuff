#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#define NUM_LOOKUPS 0x7
#define INBUF_SIZE 0x20

static const char *_s_host_names[NUM_LOOKUPS] = {
	"www.google.com"
	, "smpp-04.lab1.bandwidthclec.com"
	, "mms-01a.dfw1.bandwidthclec.com"
	, "www.bandwidth.com"
	, "www.facebook.com"
	, "api.catapult.inetwork.com"
	, "foo.bar.baz.de"
};

static struct gaicb _s_lookups[NUM_LOOKUPS];
static struct gaicb *_s_lookup_ptrs[NUM_LOOKUPS];

static struct sigevent _s_event;

static volatile bool _s_running = true;

/**
 * takes void * and returns void * to work with pthreads, I guess
 */
void* resolver_loop(void *);

/* according to man page all values should be zero except
 * flags, family, socktype and protocol in hints */
static const struct addrinfo _s_hints = {
	.ai_flags = 0 /* we don't want AI_PASSIVE or anything */
	, .ai_family = AF_INET /* IPV4 only */
	, .ai_socktype = SOCK_STREAM /* tcp */
	, .ai_protocol = 0 /* any protocol */
	, .ai_addrlen = 0
	, .ai_addr = NULL
	, .ai_canonname = NULL
	, .ai_next = NULL
};

int main(int argc, char *argv[])
{
	int result = 0;
	pthread_t loop_thread;
	char inbuf[INBUF_SIZE];

	memset(&_s_event, 0, sizeof(_s_event));
	_s_event.sigev_notify = SIGEV_NONE;

	for (int i = 0 ; i < NUM_LOOKUPS ; i++) {
		_s_lookup_ptrs[i] = &_s_lookups[i]; /* _s_lookups + i */

		_s_lookups[i].ar_name = _s_host_names[i];
		_s_lookups[i].ar_service = NULL; /* don't need port number to resolve host */
		_s_lookups[i].ar_request = &_s_hints;
		_s_lookups[i].ar_result = NULL; /* we don't need to initialize it, but set to null anyway */
	}

	result = getaddrinfo_a(GAI_NOWAIT, _s_lookup_ptrs, NUM_LOOKUPS, &_s_event);
	if (result) {
		dprintf(2, "failed to start requests, %s\n", gai_strerror(result));
		exit(1);
	}

	result = pthread_create(&loop_thread, NULL, resolver_loop, NULL);
	if (result) {
		errno = result;
		perror("failed to start thread\n");
		exit(1);
	}

	/* read anything from stdin to decide to kill program */
	result = read(0, inbuf, INBUF_SIZE);
	if (result == -1) {
		/* what */
		exit(1);
	}
	_s_running = false;
	result = pthread_join(loop_thread, NULL);
	if (result) {
		errno = result;
		perror("failed to join thread\n");
		exit(1);
	}
	return 0;
}

void* resolver_loop(void *arg)
{
	int suspend_result = 0
		, gai_error_result = 0;

	struct timespec timeout = {
		.tv_sec = 5,
		.tv_nsec = 0
	};
	struct timespec sleeptime = {
		.tv_sec = 1,
		.tv_nsec = 0
	};

	char addrstr_buf[INET_ADDRSTRLEN + 1];
	addrstr_buf[INET_ADDRSTRLEN] = 0;
	char *inet_ntop_result;

	while (_s_running) {
		suspend_result = gai_suspend(_s_lookup_ptrs, NUM_LOOKUPS, &timeout);
		if (suspend_result == EAI_AGAIN || suspend_result == EAI_INTR) {
			continue;
		}
		if (suspend_result == EAI_ALLDONE) {
			/* TODO gwthread_sleep or whatever (block on wakefd)
			 * in actual usage, whatever threads were requesting lookups would be modifying the contents
			 * of the lookup list */
			dprintf(2, "all done, sleeping\n");
			/* ignore error sleeping I guess */
			nanosleep(&sleeptime, NULL);
			
		}

		if (suspend_result == 0) {
			/* TODO number of requests to iterate may change?*/
			for (int i = 0; i < NUM_LOOKUPS; i++) {
				gai_error_result = gai_error(_s_lookup_ptrs[i]);
				if (gai_error_result == EAI_INPROGRESS) {
					continue;
				}
				if (gai_error_result == 0) {
					/* TODO callback to send to whoever wanted it
					 * will need to wrap the struct gaicb in a custom struct for this data */
					struct addrinfo *completed_addrinfo = _s_lookups[i].ar_result;
					inet_ntop_result = inet_ntop(completed_addrinfo->ai_family
							, &((struct sockaddr_in *) completed_addrinfo->ai_addr)->sin_addr
							, addrstr_buf
							, completed_addrinfo->ai_addrlen);
					if (inet_ntop_result == NULL) {
						perror("error in inet_ntop\n");
						continue;
					}

					printf("resolved %s to %s\n", _s_lookups[i].ar_name, addrstr_buf);
					continue;
				}
				/* if we reached this point, gai_error_result is an actual failure */
				dprintf(2, "error looking up %s: %s\n", _s_lookups[i].ar_name, gai_strerror(gai_error_result));
			}
		}
	}
}
