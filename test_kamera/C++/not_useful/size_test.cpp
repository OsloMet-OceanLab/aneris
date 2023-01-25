#include <iostream>
#include <opencv2/opencv.hpp>
#include <fstream>

int main(int argc, char** argv, char** envp)
{
	cv::VideoCapture cap(0, cv::CAP_V4L2);

	if (!cap.isOpened())
	{
		fputs("Could't open the camera feed\n", stderr);
		return -1;
	}

	cap.set(3, 1920);
	cap.set(4, 1080);
//	cap.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));

	printf("Press the escape key to quit\n");

	double dWidth = cap.get(cv::CAP_PROP_FRAME_WIDTH);
	double dHeight = cap.get(cv::CAP_PROP_FRAME_HEIGHT);

	printf("Resolution: %f x %f\n", dWidth, dHeight);

	const char *window_title = "Camera feed";
	cv::namedWindow(window_title);

	cv::Mat frame = cv::Mat::zeros(480, 640, CV_8UC1), greyFrame = cv::Mat::zeros(480, 640, CV_8UC1);
	
	bool bSuccess = cap.read(frame);
	if (!bSuccess)
	{
		fputs("Can't get feed from camera\n", stderr);
		return -2;
	}
	
	std::ofstream color("/home/pi/Desktop/color", std::ios_base::out);
	std::ofstream grey("/home/pi/Desktop/grey", std::ios_base::out);
	
	cv::cvtColor(frame, greyFrame, cv::COLOR_BGR2GRAY);

	color << frame.data;
	grey << greyFrame.data;
	
	color.close();
	grey.close();
	
	return 0;
}
