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

    cv::Mat frame, gray, binary, morphopen, morphclose, erode, dilated;
    std::vector<std::vector<cv::Point>> contours;

    while(true)
    {
        cap >> frame;
        if(frame.empty()) break;

        // Step 1: Convert to gray
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

        // Step 2: Thershold
        cv::threshold(gray, binary, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

        // Step 3: Find contours
        cv::findContours(binary, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        // Step 4: Draw Contours
        cv::drawContours(frame, contours, -1, cv::Scalar(0, 0, 255), 2);

        cv::imshow("Contours", frame);

        if(cv::waitKey(1) == 27) break;
    }

    return(0);


}