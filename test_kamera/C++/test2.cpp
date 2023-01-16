#include <iostream>
#include <opencv2/opencv.hpp>

//using namespace cv;

int main(void)
{
	cv::VideoCapture cap(0, cv::CAP_V4L2);

	if (!cap.isOpened())
	{
		fputs("Could't open the camera feed\n", stderr);
		return -1;
	}

	printf("Press the escape key to quit\n");

	double dWidth = cap.get(cv::CAP_PROP_FRAME_WIDTH);
	double dHeight = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
	//double dWidth = 4;
	//double dHeight = 4;

	printf("Resolution: %f x %f\n", dWidth, dHeight);

	char *window_title = "Camera feed";
	//cv::namedWindow(window_title);

	int index = 0;
	cv::Mat frame;
	bool bSuccess = false;

	while(true)
	{
		bSuccess = cap.read(frame);
		if (!bSuccess)
		{
			fputs("Can't get feed from camera\n", stderr);
			cap.release();
			//return -2;
			cv::VideoCapture cap(0, cv::CAP_V4L2);
			bSuccess = cap.read(frame);
			index++;
			if(index>10)
				return -2;
		}

		cv::imshow(window_title, frame);

		if(cv::waitKey(10) == 27)
		{
			printf("Closing...\n");
			cap.release();
			return 0;
		}
	}
	
	
	
	return 0;
}
