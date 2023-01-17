#include <iostream>
#include <opencv2/opencv.hpp>
#include <unistd.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT	5000

int main(int argc, char** argv)
{
	// set up opencv
	const char *window_title = "Camera feed";
	cv::namedWindow(window_title);

	cv::Mat frame;
	
	// set up socket
	int sockfd;
	struct sockaddr_in servaddr;
	
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd < 0)
	{
		fputs("Couldn't create a socket\n", stderr);
		return 1;
	}
	
	memset(&servaddr, 0, sizeof(servaddr));
	
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(PORT);
	
	int n, len;

	while(true)
	{
		n = recvfrom(sockfd, (void*)&frame, sizeof(frame), MSG_WAITALL, (struct sockaddr*)&servaddr, (socklen_t*)&len);
		cv::imshow(window_title, frame);

		if(cv::waitKey(10) == 27)
		{
			printf("Closing...\n");
			close(sockfd);
			return 0;
		}
	}
}

