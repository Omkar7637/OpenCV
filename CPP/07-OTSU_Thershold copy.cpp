#include<opencv2/opencv.hpp>
#include<iostream>

int main()
{
    cv::VideoCapture cap(
    "nvarguscamerasrc ! video/x-raw(memory:NVMM), width=640, height=480, framerate=60/1 ! "
    "nvvidconv ! video/x-raw, format=BGRx ! videoconvert ! video/x-raw, format=BGR ! appsink",
    cv::CAP_GSTREAMER
    );

    if(!cap.isOpened())
    {
        std::cout << "Camera Not supported!" << std::endl;
        return -1;
    }

    cv::namedWindow("Camera", cv::WINDOW_NORMAL);
    cv::setWindowProperty("Camera", cv::WND_PROP_FULLSCREEN, cv::WINDOW_FULLSCREEN);

    cv::Mat frame, gray, binary;

    while(true)
    {
        cap >> frame;
        if(frame.empty()) break;

        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

        cv::threshold(gray, binary, 0, 255, cv::THRESH_BINARY_INV | cv::THRESH_OTSU | cv::THRESH_BINARY);

        cv::imshow("Grayscale", gray);
        cv::imshow("Binary", binary);

        if(cv::waitKey(1) == 27) break;
    }

    return(0);


}