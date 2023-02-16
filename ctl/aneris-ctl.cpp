#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/var/run/aneris.sock"
#define COMMAND_SIZE 4

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		printf("Usage: %s <option>\n", argv[0]);
		exit(1);
	}
	
	int sock;
	struct sockaddr_un serv;
	char buf[COMMAND_SIZE];
	memset(&serv, 0, sizeof(struct sockaddr_un));
	
	sock = socket(AF_UNIX, SOCK_DGRAM, 0);
	serv.sun_family = AF_UNIX;
	strcpy(serv.sun_path, SOCKET_PATH);
	
	strncpy(buf, argv[1], COMMAND_SIZE);
	sendto(sock, buf, strlen(buf), 0, (struct sockaddr *) &serv, sizeof(serv));
	close(sock);
	return 0;
}
