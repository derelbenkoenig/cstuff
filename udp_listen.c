#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

#define BUFSIZE 512
#define PORT 10123
#define ADDR_SIZE (sizeof(struct sockaddr_in))

void fail(char *s)
{
        perror(s);
        exit(1);
}

int main(int argc, char *argv[])
{
	struct sockaddr_in our_addr, other_addr;
	int sockfd;
	socklen_t other_addrsize; 
	size_t received_size;
	char recv_buf[BUFSIZE];
	char addrstr[32];

	if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
		fail("couldn't create socket");
	}

	memset(&our_addr, 0, ADDR_SIZE);
	our_addr.sin_family = AF_INET;
	our_addr.sin_port = htons(PORT);
	our_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if ((bind(sockfd, (struct sockaddr *) &our_addr, ADDR_SIZE)) == -1) {
		fail("could not bind to address");
	}

	/* receive data */
	for (;;) {
		printf("Waiting for message\n");
		received_size = recvfrom(sockfd
				, recv_buf
				, BUFSIZE
				, 0
				, (struct sockaddr *) &other_addr
				, &other_addrsize);
		if (received_size == -1) {
			fail("could not receive message");
		}
		inet_ntop(AF_INET, &other_addr.sin_addr, addrstr, other_addrsize);
		printf("===> Connection from %s:%d\n", addrstr, other_addr.sin_port);
		write(1, recv_buf, received_size);
		printf("\n");
	}

	close(sockfd);
	return 0;
}
