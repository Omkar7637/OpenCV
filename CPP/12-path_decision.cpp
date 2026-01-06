/************************************************************************************************************
*   File Name   : 12-Path_decision.cpp
*   Discription : Path direction decision (Left/Center/Right) Robot should go Left, straight, or right.
*                 vision + decision
*                 # Human Thinking vs Robot thinking
*                       Human: "The path is more open on right, I'll turn right."
*                       Robot: "There are more WHITE pixel on the right."
*                   So we convert intuition -> Numbers
*                 # Divide the ROI into 3 Zones
*                       | LEFT | CENTER | RIGHT |
*                       Each zone:  Count WHITE pixels
*                                   More White = More free space
*   Author      : Omkar Ankush Kashid
*   Created On  : 06-01-2026
*   Last updated: 06-01-2026
*   Platform    : NVIDIA Jetson Nano Super 8GB
*   OS          : C++   

************************************************************************************************************/

// ==================== HEADER FILES ====================
#include <opencv2/opencv.hpp> 
#include <iostream>

int main()
{
    // Local Variable decalarations
    // OpenCV Related variable 
    cv::Mat frame, gray, binary, roi;

    // ========== CSI Camera Pipeline ==========
    // CSI Camera pipeline required for jetson nano
    std::cout << std::endl;
    std::cout << "----- IGV::Pipeline Intialization Start -----" << std::endl;
    cv::VideoCapture cap(
        "nvarguscamerasrc !" // Direct interface to nividia driver (Auto setting)
        "video/x-raw(memory:NVMM)," // Nvidia multimedia memory
        "width=1920, height=1080, frame=60/1 !"
        "nvvidconv !" // use jetson hardware | Converter: Memory Layout, color formats, resolutions
        "video/x-raw, format=BGRx !" // BGR + unused alpha channel required videoconverter works efficently with BGRx
        "videoconvert !" // CPU based color format converter BGRx -> BGR OpenCV expects BGR format
        "video/x-raw, format=BGR !" // Final format passed to the OpenCV | 3-Channel BGR image (CV_8UC3)
        " appsink ", // Sink elements that allows application (OpenCV to read frames | Without appsink, OpenCV cannot access the Video Stream)

        // ========== Backend Selection ==========
        cv::CAP_GSTREAMER // Force OpenCV to use the GSTREAMER backend | Mandatory when passing a GSTREAMER pipeline string
    );

    std::cout << "----- IGV::Camera Pipeline Scessfully intialized -----" << std::endl;

    // ========== Camera Access ==========
    // 
    // - GStreamer Pipeline creation | Camera avilabilty
    // - Successful negotioantion of caps (resolution, FPS, Format)
    if(!cap.isOpened()) // If any Element in the pipeline fails, this return false
    {
        std::cout << "----- IGV::Camera Not Supported!! -----" << std::endl;
        return(EXIT_FAILURE); // Exit program safely
    }

    std::cout << "----- IGV::Camera Working -----" << std::endl;

    std::cout << "===================================================" << std::endl;
    std::cout << "\n----- IGV::Pipeline -----" << std::endl;
    std::cout << "CSI Camera Sensor\n       |       \nnvarguscamerasrc\n       |       \nNVMM (GPU Memory)\n       |       \nnvvidconv (HW Accelearted)\n       |       \n      BGRx\n       |       \nvideoconvertor (CPU)\n       |       \n    appsink\n       |       \nOpenCV cv::Mat\n" << std::endl;
    std::cout << "===================================================" << std::endl;

    // While Loop for video each frame scanning
    std::cout << "----- IGV::Entering While Loop -----" << std::endl;
    while(true)
    {
        // Take a single frame 
        // Camera -> Frame 
        cap >> frame; 

        // Check if frame is empty
        if(frame.empty())
        {
            std::cerr << "IGV::ERROR::EMPTY FRAME RECEVIED!" << std::endl;
            return(EXIT_FAILURE);
        }

        // Step 1.Garyscale
        // frame -> Grayscale
        cv::cvtColor(
            frame,          
        )
        
    }

}