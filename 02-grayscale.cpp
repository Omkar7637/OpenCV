#include<opencv2/opencv.hpp>
#include<iostream>

int main()
{
	cv::Mat image = cv::imread("test.jpg");

	if(image.empty())
	{
		std::cout << "Image Not Loaded!" << std::endl;
		return -1;
	}

	cv::Mat gray;
	cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
	
	cv::Mat hsv;
	cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);

	cv::imshow("Original", image);
	cv::imshow("Grayscale", gray);
	cv::imshow("HSV", hsv);

	// Split channels 
	std::vector<cv::Mat> channels;
	cv::split(image, channels);

	cv::imshow("Blue", channels[0]);
	cv::imshow("Green", channels[1]);
	cv::imshow("Red", channels[2]);

	// HSV split
	cv::split(hsv, channels);
	cv::imshow("Hue", channels[0]);

	cv::waitKey(0);
	return 0;
}
