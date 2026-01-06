/*************************************************************************************
 *  File Name   : 13-Motion_stop.cpp
 *  Description : Motion Dectection using fame Differencing
 *                  Purpose:    
 *                      - Detect sudden motion in front of the robot
 *                      - Trigger EMERGENCY STOP if motion exceeds a thershold
 *                  Technique Used:
 *                      - Grayscale conversion
 *                      - Gaussion blur (noise reduction)    
 *                      - Frame differencing
 *                      - Binary thresholding   
 *                      - Motion pixel counting
 * 
*************************************************************************************/

#include <opencv2/opencv.hpp>
#include <iostream>
#include <cstdlib>

int main()
{
    // ==================== IMAGE CONTAINERS ====================
    cv::Mat frame;      // Orignal color image from camera (BGR)
    cv::Mat gray;       // current grayscale frame
    cv::Mat prevGray;   // previous grayscale frame (used for comparision)
    cv::Mat diff;       // absolute difference between current and prevoiuos frames
    cv::Mat binary;     // thersholded motion mask (white = motion)
    
    // Flag to handle the very first frame
    // firstFrame = true means:
    //      - No prevoius frame exists yet
    //      - Motion detection cannot be done
    bool firstFame = true;

    // ==================== CODE ====================
    // ========== CSI CAMERA PIPELINE ==========
    // CSI camera pipeline required for the jetson nano
    std::cout << std::endl;
    std::cout << "----- IGV::Pipline intialization Start -----" << std::endl;
    
    // Video Capture class
    cv::VideoCapture cap(
        "nvarguscamerasrc !" // Direct interface to the nvidia hardware (mostly auto setting)
        "video/x-raw(memory:NVMM), " // NVIDIA multimedia memory
        "width=1920, height=1080, frame=60/1 !"
        "nvvidconv !" // Uses jetson hardware | much faster for the video converter 
        ""
    )

    return (EXIT_SUCCESS);
}