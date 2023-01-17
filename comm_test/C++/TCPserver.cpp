#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080

int main(int argc, char** argv)
{
	int server_fd, new_socket, valread;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[1024] = { 0 };
	char* msg = "Message from the server";
	
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(server_fd < 0)
	{
		fputs("Can't create socket", stderr);
		return 1;
	}
	
	if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
	{
		fprintf(stderr, "Can't attach socket to port %d", PORT);
		return 2;
	}
	
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);
	
	int bound = bind(server_fd, (struct sockaddr*)&address, sizeof(address));
	
	if(bound)
	{
		fputs("Can't bind to port", stderr);
		return 3;
	}
	
	if(listen(server_fd, 3) < 0)
	{
		fputs("Can't listen to port", stderr);
		return 4;
	}
	
	new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
	if(new_socket < 0)
	{
		fputs("Can't accept new connections", stderr);
		return 5;
	}
	
	valread = read(new_socket, buffer, 1024);
	printf("%s\n", buffer);
	send(new_socket, msg, strlen(msg), 0);
	printf("Message sent\n");
	
	close(new_socket);
	shutdown(server_fd, SHUT_RDWR);
	
	return 0;
}
