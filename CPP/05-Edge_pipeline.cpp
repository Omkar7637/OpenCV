#include<opencv2/opencv.hpp>
#include<iostream>

int main()
{
    cv::VideoCapture cap(
    "nvarguscamerasrc ! video/x-raw(memory:NVMM), width=1920, height=1080, framerate=60/1 ! "
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

    cv::Mat frame, gray, blurimage, edges;

    while(true)
    {
        cap >> frame;
        if(frame.empty()) break;

        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
        cv::GaussianBlur(gray, blurimage, cv::Size(5, 5), 0);
        cv::Canny(blurimage, edges,100, 200);

        cv::imshow("Camera", edges);

        if(cv::waitKey(1) == 27) break;
    }
    return 0;
}