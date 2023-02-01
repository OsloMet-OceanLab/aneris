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
	char buffer[MAXLINE], *msg;
	struct sockaddr_in servaddr, cliaddr;
	
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	
	if(sockfd < 0)
	{
		fputs("Couldn't create a socket\n", stderr);
		return -1;
	}
	
	memset(&servaddr, 0, sizeof(servaddr));
	memset(&cliaddr, 0, sizeof(cliaddr));
	
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(PORT);
	
	if(bind(sockfd, (const struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
	{
		fputs("Couldn't bind the socket\n", stderr);
		return -2;
	}
	
	int len = sizeof(cliaddr), n, index = 0;
	
	do
	{
		n = recvfrom(sockfd, (char*)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr*)&cliaddr, (socklen_t*)&len);
		
		buffer[n] = '\0';
		
		printf("Client: %s\n", buffer);
		
		msg = "Message from server no. ";
		//msg[strlen(msg)] = index;
		
		sendto(sockfd, (const char*)msg, strlen(msg), MSG_CONFIRM, (const struct sockaddr*)&cliaddr, len);
		
		puts("Sent message\n");
		index++;
	} while (strncmp(buffer, "Quit", 1024) != 0);
	
	close(sockfd);	
	
	return 0;
}
