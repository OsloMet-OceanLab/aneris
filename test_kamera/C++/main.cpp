#include <iostream>
#include <opencv2/opencv.cpp>

//using namespace cv;

int main(void)
{
	cv::VideoCapture cap(0);

	if (!cap.isOpened())
	{
		fputs("Could't open the camera feed\n", stderr);
		return -1;
	}

	printf("Press the escape key to quit\n");

	double dWidth = cap.get(CAP_PROP_FRAME_WIDTH);
	double dHeight = cap.get(CAP_PROP_FRAME_HEIGHT);

	printf("Resolution: %d x %d\n", dWidth, dHeight);

	char *window_title = "Camera feed";
	cv::namedWindow(window_name);

	while(true)
	{
		cv::Mat frame;
		bool bSuccess = cap.read(frame);
		if (!bSuccess)
		{
			fputs("Camera is not connected\n", stderr);
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
