#include <opencv2/opencv.hpp>
#include <iostream>

int main()
{
	// Read image from file
	cv::Mat image = cv::imread("test.jpg");

	// Check if image loaded
	if(image.empty())
	{
		std::cout << "Image not Loaded!" << std::endl;
		return -1;
	}
	std::cout << image.rows << std::endl; // height
	std::cout << image.cols << std::endl; // width
	std::cout << image.channels() << std::endl; // 3 = BGR
	
	cv::Vec3b pixel = image.at<cv::Vec3b>(100, 50);
	std::cout << "Blue: " << (int)pixel[0] << std::endl;
	std::cout << "Green: " << (int)pixel[1] << std::endl;
	std::cout << "Red: " << (int)pixel[2] << std::endl;


	// Draw on image 
	cv::circle(image, cv::Point(100, 100), 50, cv::Scalar(0, 0, 255), 2);
	cv::rectangle(image, cv::Point(50, 50), cv::Point(200, 200), cv::Scalar(255, 0, 0), 2);
	cv::line(image, cv::Point(0, 0), cv::Point(300, 300), cv::Scalar(0, 255, 0), 2);

	// Display iamge
	cv::imshow("My First OpenCV Winodw", image);

	// Wait until key press
	cv::waitKey(0);

	return 0;
}
