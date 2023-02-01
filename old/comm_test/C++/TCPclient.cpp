#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT	8080

int main(int argc, char** argv)
{
	int sock = 0, valread, client_fd;
	struct sockaddr_in serv_addr;
	const char* msg = argc > 1 ? argv[1] : "Message from client";
	char buffer[1024] = { 0 };
	
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0)
	{
		fputs("Can't create socket", stderr);
		return 1;
	}
	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	
	int addr_assignment = inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
	if(addr_assignment <= 0)
	{
		fputs("Invalid address", stderr);
		return 2;
	}
	
	client_fd = connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	if(client_fd < 0)
	{
		fputs("Connection failed", stderr);
		return 3;
	}
	
	send(sock, msg, strlen(msg), 0);
	printf("Message sent\n");
	valread = read(sock, buffer, 1024);
	printf("%s\n", buffer);
	
	close(client_fd);
	
	return 0;
}
