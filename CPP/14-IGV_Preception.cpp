/*****************************************************************************************
 *  File Name   : 14-IGV_Preception.cpp
 *  Project     : IGV Vision System - Path Detection + Emergency Stop
 *  Language    : C++
 *  Library     : OpenCV
 * 
 *  Description :
 *      - Capture live camera feed
 *      - Performs preprocessing (Grayscale + blur)
 *      - Detects sudden motion for EMERGENCY STOP
 *      - Detects path direction using ROI zoning
 *      - Displays decision on live video
 * 
 *  Designed for : 
 *      - Mobile Robotics / IGV / AGV
 *      - Embedded vision systems
 * 
 *  Author      : Omkar Ankush Kashid
 *  Created on  : 07-01-2026
 *  Last Updated: 07-01-2026
 *  Platform    : NVIDIA JETSON NANO SUPER 8GB
 *  OS          : Ubuntu (JetPack)
 *  Framework   : OpenCV 4.x, GStreamer
 *  Version     : 1.0.14
 *  Notes       : 
 *      - Uses nearguscamerasrc for CSI
 *      - Requires OpenCV built with GStreamer support.
 *      - Designed for embedded vision applications.
*****************************************************************************************/

// ============================== HEADER FILES ==============================
#include<opencv2/opencv.hpp>
#include<iostream>
#include<cstdlib>

// ========================== ENTRY POINT FUNCTION ==========================
int main()
{
    // ==================== CAMERA INTIALIZATION ====================
    
    std::cout << std::endl << "========== Camera Intitializations ==========" << std::endl;
    cv::VideoCapture cap(
        "nvarguscamerasrc !" // Direct interface to nividia hardware (Mostly auto setting)
        "video/x-raw(memory:NVMM), " // NVIDIA Multimedia memory
        "width=1280, height=720, frame=60/1 !"
        "nvvidcon !" // Uses Jetson hardware | Much faster for video converter
        "video/x-raw, format=BGRx !" //  BGR + usused alpha channel required | Video Converter work efficently with BGRx
        "videoconvert !" // CPU-Based color format converter | Convert BGRx -> BGR | OpenCV expects BGR format
        "video/x-raw"
    )



    return(EXIT_SUCCESS);
}