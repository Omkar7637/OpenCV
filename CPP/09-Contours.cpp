#include<opencv2/opencv.hpp>
#include<iostream>

int main()
{
    // cv::VideoCapture cap(
    // "nvarguscamerasrc ! video/x-raw(memory:NVMM), width=640, height=480, framerate=60/1 ! "
    // "nvvidconv ! video/x-raw, format=BGRx ! videoconvert ! video/x-raw, format=BGR ! appsink",
    // cv::CAP_GSTREAMER
    // );

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

    cv::Mat frame, gray, binary, morphopen, morphclose, erode, dilated;
    std::vector<std::vector<cv::Point>> contours;

    cv::Mat kernal = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));

    while(true)
    {
        cap >> frame;
        if(frame.empty()) break;


        // Step 1: Convert to gray
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

        // Step 2: Thershold
        cv::threshold(gray, binary, 0, 255, cv::THRESH_BINARY_INV | cv::THRESH_OTSU);

        cv::morphologyEx(binary, morphclose, cv::MORPH_CLOSE, kernal);

        // Step 3: Find contours
        cv::findContours(morphclose, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        // Step 4: Draw Contours
        cv::drawContours(frame, contours, -1, cv::Scalar(0, 0, 255), 2);

        for(size_t i = 0; i < contours.size(); i++)
        {
            // Compute contour area
            double area = cv::contourArea(contours[i]);

            // Ignore small contours (noise)
            if(area < 500)
                continue;

            // Compute movements
            cv::Moments m = cv::moments(contours[i]);
            cv::Rect box = cv::boundingRect(contours[i]);
            // Safety check (avoid divide by zero)
            if (m.m00 == 0)
                continue;

            // Compute centroid
            int cx = static_cast<int>(m.m10 / m.m00);
            int cy = static_cast<int>(m.m01 / m.m00);

            // Draw centroid
            cv::circle(frame, cv::Point(cx, cy), 5, cv::Scalar(0, 255, 0), -1);

            // Draw contours
            cv:: drawContours(frame, contours, i, cv::Scalar(0, 0 ,255), 2);

            // Draw bonding Box
            cv::rectangle(frame, box, cv::Scalar(255, 0, 0), 2);

            // Show centroid coordinates
            cv::putText(frame, "Cx:" + std::to_string(cx) + " Cy:" + std::to_string(cy) + "Area: " + std::to_string(area),
            cv::Point(cx + 10, cy - 10), 
            cv::FONT_HERSHEY_SIMPLEX,
            0.5, cv::Scalar(0, 255, 0), 
            1);

            // std::cout << area << std::endl;
        }

        cv::imshow("Camera", frame);

        if(cv::waitKey(1) == 27) break;
    }

    return(0);


}
