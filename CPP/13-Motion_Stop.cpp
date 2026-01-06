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
    cv::Mat frame; // Orignal color image from camera (BGR)
    cv::Mat gray;  // current grayscale frame
    cv::Mat prevGray // previous grayscale frame (used for comparision)
    // 

    return (EXIT_SUCCESS);
}