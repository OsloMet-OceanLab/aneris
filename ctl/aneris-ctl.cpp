#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/var/run/aneris.sock"
#define COMMAND_SIZE 2

void printHelp(char *pName);

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		printHelp(argv[0]);
		exit(1);
	}
	
	if (!strncmp("--help", argv[1], 6))
	{
		printHelp(argv[0]);
		exit(0);
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

void printHelp(char *pName)
{
	printf("Usage: %s <option>\n", pName);
	printf("Possible choices:\n");
	printf("1\tPower device off\n");
	printf("2\tReboot device\n");
	printf("3\tTurn lights on\n");
	printf("4\tTurn lights off\n");
	printf("5\tTurn wipers on\n");
	printf("6\tTurn wipers off\n");
	printf("7\tTurn porpoise on\n");
	printf("8\tTurn wipers off\n");
	printf("9\tClear log file\n");
	printf("10\tStart web server\n");
	printf("11\tTerminate web server\n");
}
