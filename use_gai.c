#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

static const char *hostname = "www.facebook.com";

int main (int argc, char *argv[])
{
	struct addrinfo hints = {
		.ai_flags = 0 /* we don't want AI_PASSIVE or anything */
		, .ai_family = AF_INET /* IPV4 only */
		, .ai_socktype = SOCK_STREAM /* tcp */
		, .ai_protocol = 0 /* any protocol */
		, .ai_addrlen = 0
		, .ai_addr = NULL
		, .ai_canonname = NULL
		, .ai_next = NULL
	};
	struct addrinfo *result_ptr;
	int result = 0;
	char addrstr_buf[INET_ADDRSTRLEN + 1];
	addrstr_buf[INET_ADDRSTRLEN] = 0;
	char *inet_ntop_result;

	result = getaddrinfo(hostname, NULL, &hints, &result_ptr);
	if (result) {
		dprintf(2, "error in getaddrinfo: %s\n", gai_strerror(result));
		exit(1);
	}

	inet_ntop_result = inet_ntop(result_ptr->ai_family
			, &((struct sockaddr_in *) result_ptr->ai_addr)->sin_addr
			, addrstr_buf
			, result_ptr->ai_addrlen);
	if (inet_ntop_result == NULL) {
		perror("error in inet_ntop\n");
		exit(1);
	}

	printf("resolved %s to %s\n", hostname, addrstr_buf);
}
