#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/var/run/aneris.sock"
#define COMMAND_SIZE 2

int main(int argc, char **argv)
{
	if (argc != 2 || !strncmp("--help", argv[1], 6))
	{
		printf("Usage: %s <option>\n", argv[0]);
		printf("Possible choices:\n");
		printf("1\tpower device off\n");
		printf("2\treboot device\n");
		printf("3\tturn lights on\n");
		printf("4\tturn lights off\n");
		printf("5\tturn wipers on\n");
		printf("6\tturn wipers off\n");
		printf("7\tclear log file\n");
		printf("8\tstart web server\n");
		printf("9\tterminate web server\n");
		exit(argc != 2 ? 1 : 0);
	}
	
	int sock;
	struct sockaddr_un serv;
	char buf[COMMAND_SIZE];
	memset(&serv, 0, sizeof(struct sockaddr_un));
	
	sock = socket(AF_UNIX, SOCK_DGRAM, 0);
	serv.sun_family = AF_UNIX;
	memcpy(serv.sun_path, SOCKET_PATH, strlen(SOCKET_PATH));
	
	memcpy(buf, argv[1], COMMAND_SIZE);
	sendto(sock, buf, strlen(buf), 0, (struct sockaddr *) &serv, sizeof(serv));
	close(sock);
	return 0;
}
