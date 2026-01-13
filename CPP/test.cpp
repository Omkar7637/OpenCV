#include <opencv2/opencv.hpp>
#include <iostream>

int main()
{
    cv::VideoCapture cap(
        "nvarguscamerasrc ! "
        "video/x-raw(memory:NVMM), width=640, height=480, framerate=60/1 ! "
        "nvvidconv ! video/x-raw, format=BGRx ! "
        "videoconvert ! video/x-raw, format=BGR ! "
        "appsink drop=true sync=false",
        cv::CAP_GSTREAMER
    );

    if (!cap.isOpened())
    {
        std::cout << "Camera not opened!" << std::endl;
        return -1;
    }

    cv::Mat frame, gray, blurImg, edges;

    while (true)
    {
        cap >> frame;
        if (frame.empty()) break;

        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
        cv::GaussianBlur(gray, blurImg, cv::Size(5,5), 0);
        cv::Canny(blurImg, edges, 50, 150);

        cv::imshow("Original", frame);
        cv::imshow("Edges", edges);

        if (cv::waitKey(1) == 27) break;
    }

    return 0;
}