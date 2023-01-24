#include <iostream>
#include <opencv2/opencv.hpp>
#include <unistd.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT	5000

int recvall(const int sockfd, void __restrict *buf, int *buf_len, const struct sockaddr *servaddr, socklen_t *servaddr_len);

int main(int argc, char** argv)
{
	// set up opencv
	const char *window_title = "Camera feed";
	cv::namedWindow(window_title);

	cv::Mat frame;
	
	printf("Setup opencv");
	
	// set up socket
	int sockfd;
	struct sockaddr_in servaddr;
	
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd < 0)
	{
		fputs("Couldn't create a socket\n", stderr);
		return 1;
	}
	
	printf("Setup socket");
	
	memset(&servaddr, 0, sizeof(servaddr));
	
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(PORT);
	
	int n, servaddr_len;
	
	printf("Begin loop");

	while(true)
	{
		n = recvall(sockfd, frame.data, &frameSize, (const struct sockaddr*)&servaddr, (socklen_t*)&servaddr_len);
		cv::imshow(window_title, frame);

		if(cv::waitKey(10) == 27)
		{
			printf("Closing...\n");
			close(sockfd);
			return 0;
		}
	}
}

int recvall(const int sockfd, void __restrict *buf, int *buf_len, const struct sockaddr *servaddr, socklen_t *servaddr_len)
{
	if(recv(sockfd, buf_len, sizeof(int), MSG_WAITALL) < 0) return -1;
	
	int total = 0, bytes_left = *buf_len, n = 0;

	while (total < *buf_len)
	{
		n = recvfrom(sockfd, buf + total, (bytes_left > 1024 ? 1024 : bytes_left), MSG_WAITALL, servaddr, servaddr_len);
		if (n == -1) break;
		total += n;
		bytes_left -= n;
	}

	*buf_len = total;
	return n == -1 ? -1 : 0;
}
