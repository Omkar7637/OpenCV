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

    while(true)
    {
        cap >> frame;
        if(frame.empty()) break;

        cv:: Mat kernal = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));

        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

        cv::threshold(gray, binary, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

        cv::morphologyEx(binary, morphclose, cv::MORPH_CLOSE, kernal);
        cv::morphologyEx(binary, morphopen, cv::MORPH_OPEN, kernal);

        cv::erode(binary, erode, kernal);

        cv::dilate(binary, dilated, kernal);

        cv::imshow("Grayscale", gray);
        cv::imshow("Binary", binary);
        cv::imshow("After Morphology Open", morphopen);
        cv::imshow("After Morphology Close", morphclose);
        // cv::imshow("Erode", erode);
        // cv::imshow("Dilated", dilated);

        if(cv::waitKey(1) == 27) break;
    }

    return(0);


}