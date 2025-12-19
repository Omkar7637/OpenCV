#include<opencv2/opencv.hpp>
#include<iostream>

int main()
{
	// cv::VideoCapture cap(0); // 0 = deafult USB Camrea
	
//	cv::VideoCapture cap( "nvarguscamerasrc ! video/x-raw(memory:NVMM), width=1920, height=1080, freamerate=60/1 ! " "nvvidconv ! video/x-raw, format=BGRx ! videoconvert ! video/x-raw, format=BGR ! appsink", cv::CAP_GSTREAMER);
	cv::VideoCapture cap(
		"nvarguscamerasrc ! video/x-raw(memory:NVMM), width=1920, height=1080, framerate=60/1 ! "
		"nvvidconv ! video/x-raw, format=BGRx ! videoconvert ! video/x-raw, format=BGR ! appsink",
		cv::CAP_GSTREAMER
);
	
	if(!cap.isOpened())
	{
		std::cout << "Camera not opened!" << std::endl;
		return -1;
	}

	cv::namedWindow("Camera", cv::WINDOW_NORMAL);
	cv::setWindowProperty("Camera", cv::WND_PROP_FULLSCREEN, cv::WINDOW_FULLSCREEN);

	cv::Mat frame, gray, hsv, hsv_bgr;
	bool showGray = false;
	char mode  = 'b'; //  b = color g = gray, h = hsv
	cap >> frame;
	while (true)
	{
		cap >> frame;
		if(frame.empty()) break;

		char key;
		
		key = cv:: waitKey(1);

		if(key == 'g' || key == 'b' || key == 'h')
		{
			mode = key;
		}
		else if (key == 27)
		{
			break;
		}

		if(mode == 'g')
		{
                        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
                        cv::imshow("Camera", gray);

		}
		else if(mode == 'h')
		{
			cv::cvtColor( frame, hsv, cv::COLOR_BGR2HSV);
			cv::imshow("Camera", hsv);
		}
		else 
		{
			cv::imshow("Camera", frame);
		}
	}

	cap.release();
	cv::destroyAllWindows();
	return 0;

}
