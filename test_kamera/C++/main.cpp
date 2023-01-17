#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char** argv, char** envp)
{
	cv::VideoCapture cap(0, cv::CAP_V4L2);

	if (!cap.isOpened())
	{
		fputs("Could't open the camera feed\n", stderr);
		return -1;
	}

//	cap.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));

	printf("Press the escape key to quit\n");

	double dWidth = cap.get(cv::CAP_PROP_FRAME_WIDTH);
	double dHeight = cap.get(cv::CAP_PROP_FRAME_HEIGHT);

	printf("Resolution: %f x %f\n", dWidth, dHeight);

	const char *window_title = "Camera feed";
	cv::namedWindow(window_title);

	cv::Mat frame;
	bool bSuccess = false;

	while(true)
	{
		bSuccess = cap.read(frame);
		if (!bSuccess)
		{
			fputs("Can't get feed from camera\n", stderr);
			return -2;
		}

		cv::imshow(window_title, frame);

		if(cv::waitKey(10) == 27)
		{
			printf("Closing...\n");
			return 0;
		}
	}
}
