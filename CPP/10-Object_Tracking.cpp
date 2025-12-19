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

    // Create a structuring element (also called a kernel) for morphology
    //
    // cv::Mat kernel
    //  - cv::Mat is OpenCV's matrix class
    //  - Here it stores a small 2D binary mask used in morphological operations
    //
    // cv::getStructuringElement()
    //  - OpenCV function to generate predefined kernel shapes
    //
    // cv::MORPH_RECT
    //  - Shape of the kernel
    //  - MORPH_RECT  -> Rectangular kernel (all ones)
    //  - Other options:
    //      cv::MORPH_ELLIPSE -> Oval / circular shape
    //      cv::MORPH_CROSS   -> Cross (+) shape
    //
    // cv::Size(5, 5)
    //  - Kernel width  = 5 pixels
    //  - Kernel height = 5 pixels
    //  - Must usually be odd numbers (3,5,7...) so there is a center pixel
    //
    // Result:
    //  - A 5x5 matrix filled with 1s (for MORPH_RECT)
    cv::Mat kernel = cv::getStructuringElement(
        cv::MORPH_RECT,
        cv::Size(5, 5)
    );


    // While loop for video each frame scanning
    while(true)
    {
        // Take Single frame
        cap >> frame;

        // Convert a color image (BGR format) into a grayscale
        //
        // cv::cvtColor(
        //  InputArray scr,     -> source Image (Color Image)
        //  OutputArray dst,    -> Destination image (Grayscale image)
        //  int code            -> Color conversion Mode
        // )

        cv::cvtColor(
            frame, // -> Input: Origianl BGR image from camera
            gray, // Output: Single-channel
            cv::COLOR_BGR2GRAY);

        // Convert Grayscale image into only TWO Values
        // - 0   (Black)
        // - 255 (White)

    }

    
    

    return 0;
}