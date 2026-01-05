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

    // ============ CSI Camera Pipeline ============
    // CSI Camera Pipeline Required for the jetson nano
    std::cout << std::endl;
    std::cout << "----- Pipeline Initialize -----" << std::endl;
    cv::VideoCapture cap(
        "nvarguscamerasrc !" // Direct interface to the navidia driver (Most setting Auto)
        "video/x-raw(memory:NVMM)," // NVIDIA 
        
    )

    // Code

}