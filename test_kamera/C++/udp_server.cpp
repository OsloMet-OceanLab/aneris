#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT	5000

int sendall(const int sockfd, void __restrict *buf, int *buf_len, const struct sockaddr *cliaddr, const int cliaddr_len);

int main(int argc, char** argv)
{
	// set up opencv
	cv::VideoCapture cap(0, cv::CAP_V4L2);

	if (!cap.isOpened())
	{
		fputs("Could't open the camera feed\n", stderr);
		return 1;
	}
	cv::Mat frame = cv::Mat::zeros(480, 640, CV_8UC1);
	
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
	
	int cliaddr_len = sizeof(cliaddr);
	int frameSize = frame.total() * frame.elemSize() * 3;
	
	printf("Start loop\n");

	do
	{
		if (!cap.read(frame))
		{
			fputs("Can't get feed from camera\n", stderr);
			return 4;
		}
		if(!frame.isContinuous()) frame = frame.clone();
		
		sendall(sockfd, frame.data, &frameSize, (const struct sockaddr*)&cliaddr, cliaddr_len);
		printf("%d\n", frameSize);
		break;
	} while (true);
	
	close(sockfd);
}

int sendall(const int sockfd, void __restrict *buf, int *buf_len, const struct sockaddr *cliaddr, const int cliaddr_len)
{
	// inform receiver of transmission size
	if(send(sock, buf_len, sizeof(int), 0) < 0) return -1;

	int total = 0, bytes_left = *buf_len, n = 0;
	
	while (total < *buf_len)
	{
		n = sendto(sockfd, buf + total, (bytes_left > 1024 ? 1024 : bytes_left), MSG_CONFIRM, cliaddr, cliaddr_len);
		if (n == -1) break;
		total += n;
		bytes_left -= n;
	}

	*buf_len = total;
	return n == -1 ? -1 : 0;
}
