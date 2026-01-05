/*****************************************************************************************
*   File Name   : Clear_Path.cpp
*   Discription : Path finding for IGV using OpenCV and GStreamer
*                This application captures frames from the Jetson Nano CSI
*                camera, processes them using OpenCV, and displays the output.
*
*   Author      : Omkar Ankush Kashid
*   Created On  : 29-12-2025
*   Last Updated: 05-01-2026
*
*   Platform    : NVIDIA Jetson Nano Super 8GB
*   OS          : Ubuntu (JetPack)
*   Language    : C++
* 
*****************************************************************************************/

// ======================== Header Files ==============================
#include <opencv2/opencv.hpp> // Realted to OpenCV 
#include <iostream> // Standered I/O stream Library

int main()
{
    // Local Variable declaartions

    // Code

    // ============ CSI Camera Pipeline ============

    // CSI Camera Pipeline Required for the jetson nano
    std::cout << std::endl;
    std::cout << "----- IGV::Pipeline Initialization Start -----" << std::endl;
    cv::VideoCapture cap(
        "nvarguscamerasrc !" // Direct interface to the navidia driver (Most setting Auto)
        
        "video/x-raw(memory:NVMM)," // NVIDIA Multimedia memory | Critical for the High FPS/Low Latency/AI Interface Pipelines
        "width=1920, height=1080, frame=60/1 ! "
        "nvvidconv !" // Uses jetson hardware | Much faster than videconverter | Converter: memory layout/color formats/resolution
        "video/x-raw, format=BGRx !" // BGR + unused alpha channel required videoconverter works efficently with BGRx 
        " videoconvert !" // CPU-based color format converter | converts BGRx -> BGR | OpenCV expects BGR format
        " video/x-raw, format=BGR !" // Final format passed to the OpenCV | 3-Channel BGR image (CV_8UC3)
        " appsink ", // Sink element that allows application (OpenCV) to read frames | Without appsink, OpenCV cannot access the video stream

        // ====== BackEnd Selection ======
        cv::CAP_GSTREAMER // Force OpenCV to use the GStreamer backend | Mandatory when passing a GStreamer pipeline string  

        
    );

    std::cout << "----- IGV::Camera Piepline Successfully intialized -----" << std::endl;

    // =============== Camera Access Check ===============
    //
    // - GStrame Pipeline creation | Camera avilabilty 
    // - Successful negotiation of caps (resolution, FPS, Format)

    if(!cap.isOpened()) // If any element in the pipeline fails, this return false
    {
        std::cout << "----- Camera Not Supported!! -----" << std::endl;
        return (EXIT_FAILURE); // Exit Program safely
    }

    std::cout <<  "----- IGV::Camera Working -----" << std::endl;

    std::cout << "----- Pipeline -----" << std::endl;
    std::cout << "CSI Camera Sensor\n       |       \nnvarguscamerasrc\n       |       \nNVMM (GPU Memory)\n       |       \nnvvidconv (HW Accelearted)\n       |       \n      BGRx\n       |       \nvideoconvertor (CPU)\n       |       \n   appsink\n       |       \nOpenCV cv::Mat\n" << std::endl;

}