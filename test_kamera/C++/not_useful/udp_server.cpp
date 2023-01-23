#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

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
	bool bSuccess = false;
	
	printf("Setup opencv\n");
	
	// set up UDP socket
	int sockfd;
	struct sockaddr_in servaddr, cliaddr;
	
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd < 0)
	{
		fputs("Couldn't create a socket\n", stderr);
		return 2;
	}
	
	printf("Setup socket\n");
	
	memset(&servaddr, 0, sizeof(servaddr));
	memset(&cliaddr, 0, sizeof(cliaddr));
	
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(PORT);
	
	if(bind(sockfd, (const struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
	{
		fputs("Couldn't bind the socket\n", stderr);
		return 3;
	}
	
	printf("Bind socket\n");
	
	int len = sizeof(cliaddr);
	
	printf("Start loop\n");

	do
	{
		bSuccess = cap.read(frame);
		if (!bSuccess)
		{
			fputs("Can't get feed from camera\n", stderr);
			return 4;
		}
		if(!frame.isContinuous())
			frame = frame.clone();
		
		//sendto(sockfd, (void*)&frame, frame.total() * frame.elemSize(), MSG_CONFIRM, (const struct sockaddr*)&cliaddr, len);
		printf("%d\n", frame.total() * frame.elemSize());
		break;
	} while (true);
	
	close(sockfd);
}
