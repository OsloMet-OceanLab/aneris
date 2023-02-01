#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT	8080
#define MAXLINE	1024

int main(int argc, char** argv)
{
	int sockfd;
	char buffer[MAXLINE];
	const char *msg = argc > 1 ? argv[1] : "Message from client";
	struct sockaddr_in servaddr;
	
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	
	if(sockfd < 0)
	{
		fputs("Couldn't create a socket\n", stderr);
		return -1;
	}
	
	memset(&servaddr, 0, sizeof(servaddr));
	
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(PORT);
	
	int n, len;
	
	sendto(sockfd, (const char*)msg, strlen(msg), MSG_CONFIRM, (const struct sockaddr*)&servaddr, sizeof(servaddr));

	puts("Message sent");
	
	n = recvfrom(sockfd, (char*)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr*)&servaddr, (socklen_t*)&len);
	
	buffer[n] = '\0';
	
	printf("Server: %s\n", buffer);
	
	close(sockfd);
	
	return 0;
}
