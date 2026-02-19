#include <opencv2/opencv.hpp>
#include <iostream>

int main()
{
    cv::VideoCapture cap(
        "nvarguscamerasrc ! "
        "video/x-raw(memory:NVMM), width=1920, height=1080, framerate=60/1 ! "
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

    cv::namedWindow("Camera", cv::WINDOW_NORMAL);
    cv::setWindowProperty(
        "Camera",
        cv::WND_PROP_FULLSCREEN,
        cv::WINDOW_FULLSCREEN
    );

    cv::Mat frame, blurImg, gussImg, medianImg;
    char mode = 'o';

    while (true)
    {
        cap >> frame;
        if (frame.empty())
            break;

        char key = cv::waitKey(1);

        if (key == 'b' || key == 'g' || key == 'm' || key == 'o')
            mode = key;
        else if (key == 27)
            break;

        if (mode == 'b')
        {
            cv::blur(frame, blurImg, cv::Size(50, 50));
            cv::imshow("Camera", blurImg);
        }
        else if (mode == 'g')
        {
            cv::GaussianBlur(frame, gussImg, cv::Size(5, 5), 0);
            cv::imshow("Camera", gussImg);
        }
        else if (mode == 'm')
        {
            cv::medianBlur(frame, medianImg, 5);
            cv::imshow("Camera", medianImg);
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
