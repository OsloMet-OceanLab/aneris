#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT	5000

int main(int argc, char** argv)
{
	// set up opencv
	const char *window_title = "Client feed";
	cv::namedWindow(window_title);

	cv::Mat frame = cv::Mat::zeros(480, 640, CV_8UC3);
	int imgSize = frame.total() * frame.elemSize() * 9;
	
	printf("Setup opencv\n");
	
	// set up socket
	int sock = 0, valread, client_fd;
	struct sockaddr_in serv_addr;
	
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0)
	{
		fputs("Can't create socket\n", stderr);
		return 1;
	}
	
	printf("Setup socket\n");
	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	
	int addr_assignment = inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
	if(addr_assignment <= 0)
	{
		fputs("Invalid address\n", stderr);
		return 2;
	}
	
	client_fd = connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	if(client_fd < 0)
	{
		fputs("Connection failed\n", stderr);
		return 3;
	}
	
	printf("Connected to server\n");
	
	printf("Begin loop\n");
	
	uchar* iptr = frame.data;
	
	while(true)
	{
		valread = recv(sock, iptr, imgSize, MSG_WAITALL);
		
		cv::imshow(window_title, frame);
		
		std::cout << valread << std::endl;
		
		memset(iptr, 0, valread);
		
		if(cv::waitKey(10) == 27)
		{
			printf("Closing...\n");
			close(client_fd);
			return 0;
		}
	}
}
