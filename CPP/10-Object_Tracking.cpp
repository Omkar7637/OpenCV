#include<opencv2/opencv.hpp>
#include<iostream>

int main()
{

    // Camera access
    cv::VideoCapture cap(
    "nvarguscamerasrc ! video/x-raw(memory:NVMM), width=1920, height=1080, framerate=60/1 ! "
    "nvvidconv ! video/x-raw, format=BGRx ! videoconvert ! video/x-raw, format=BGR ! appsink",
    cv::CAP_GSTREAMER
    );

    // Camera access cheking
    if(!cap.isOpened())
    {
        std::cout << "Camera Not supported!" << std::endl;
        return -1;
    }

    // OpenCV Variables
    cv::Mat frame, gray, binary, morph;

    // Selecting Shpe to show All 1s shpes
    cv::Mat kernal = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));

    // While loop for video each frame scanning
    while(true)
    {
        cap >> frame;
    }

    
    

    return 0;
}