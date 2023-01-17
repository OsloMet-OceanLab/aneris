#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include "Base64.hpp"

#define PORT	5000

int main(int argc, char** argv)
{
	// set up opencv
	cv::VideoCapture cap(0, cv::CAP_V4L2);

	if (!cap.isOpened())
	{
		fputs("Could't open the camera feed\n", stderr);
		return 1;
	}
	cv::Mat frame;
	frame = cv::Mat::zeros(480, 640, CV_8UC1);
	bool bSuccess = false;
	int imgSize = frame.total() * frame.elemSize();
	Base64 en;
	
	printf("Setup opencv\n");
	
	// set up TCP socket
	int server_fd, new_socket, valread;
	struct sockaddr_in address;
	int opt = 1, addrlen = sizeof(address);
	
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(server_fd < 0)
	{
		fputs("Can't create socket", stderr);
		return 1;
	}
	
	printf("Setup socket\n");
	
	if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
	{
		fprintf(stderr, "Can't attach socket to port %d", PORT);
		return 2;
	}
	
	printf("Socket attached\n");
	
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);
	
	int bound = bind(server_fd, (struct sockaddr*)&address, sizeof(address));
	
	if(bound)
	{
		fputs("Can't bind to port", stderr);
		return 3;
	}
	
	printf("Bind socket\n");
	
	if(listen(server_fd, 3) < 0)
	{
		fputs("Can't listen to port", stderr);
		return 4;
	}
	
	new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen); // ditto
	if(new_socket < 0)
	{
		fputs("Can't accept new connections", stderr);
		return 5;
	}

	
	printf("Start loop\n");

	do
	{
		bSuccess = cap.read(frame);
		if (!bSuccess)
		{
			fputs("Can't get feed from camera\n", stderr);
			return 4;
		}
		if(!frame.isContinuous()) frame = frame.clone();
		
		std::string data = Base64::Encode(frame.data);
			
		send(new_socket, (void*)&data, imgSize, 0);
		
		break;
	} while (true);
	
	close(new_socket); // might wanna move this at the end of the do loop
	
	shutdown(server_fd, SHUT_RDWR);
	return 0;
}
