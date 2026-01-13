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
 *  Author      : Omkar Ankush Kashid
 *  Created On  : 06-01-1026
 *  Last updated: 07-01-2026
 *  Platform    : NVIDIA Jetson Nano Super 8GB
 *  OS          : C++
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
    bool firstFrame = true;

    // ==================== CODE ====================
    // ========== CSI CAMERA PIPELINE ==========
    // CSI camera pipeline required for the jetson nano
    std::cout << std::endl;
    std::cout << "----- IGV::Pipline intialization Start -----" << std::endl;
    
    // Video Capture class
    cv::VideoCapture cap(
        "nvarguscamerasrc !" // Direct interface to the nvidia hardware (mostly auto setting)
        "video/x-raw(memory:NVMM), " // NVIDIA multimedia memory
        // "width=1920, height=1080, frame=60/1 !"
        "width=1280, height=720, frame=60/1 !"
        "nvvidconv !" // Uses jetson hardware | much faster for the video converter 
        "video/x-raw, format=BGRx !" // BGR + unused alpha channel required videoconverter work effiecntly with BGRx
        "videoconvert !" // CPU-based color fomat converter | Convert BGRx -> BGR | OpenCV expect BGR format
        "video/x-raw, format=BGR !" // Final format passed to the OpenCV | 3-channel BGR image (CV_8UC3)
        "appsink ", // Sink element that allows application (OpenCV) to read frames | without appsink, OpenCV cannot access the video stream

        // BACKEND SELECTION
        cv::CAP_GSTREAMER // Force OpenCV to use the GSTREAMER backend | Mandatory When passing a GSTREAMER pipeline string
    );

    std::cout << "----- IGV::Camera piline intialize succefully -----" << std::endl;

    // ========== Camera Access ==========
    // - GStreamer pipline creation | camera avilability
    // - succefull negotitation of caps (resolution, FPS, Format)

    if(!cap.isOpened()) // If any element in the pipeline fails, this return false 
    {
        std::cout << "----- IGV::Camera Not Support -----" << std::endl;
        return (EXIT_FAILURE); // Exit program safely
    }

    std::cout << "----- IGV::Camera Working -----" << std::endl;

    // While Loop for video Each Frame Scanning
    std::cout << "----- IGV::Entering while loop -----" << std::endl;

    while(true)
    {
        // Take single frame | camera -> frame
        cap >> frame;

        // Check if the frame is empty
        if(frame.empty())
        {
            std::cerr << "IGV::ERROR:Empty Frame received!" << std::endl;
            return(EXIT_FAILURE);
        }

        /***************************************************************
        *   STEP 1: Convert color image to grayscale
        *   Why: 
        *       - Motion detction depends on intensity change
        *       - Color information is unnecessary
        *       - Faster and more stable
        *   frame -> grayscale
        ***************************************************************/

       cv::cvtColor(
        frame,              // Input: Origianl BGR image from camera
        gray,               // Output: Single-cahnnel (Grayscale image)
        cv::COLOR_BGR2GRAY  // Conversion type: BGR -> Grayscale
       );

        /***************************************************************
        *   STEP 2: Gaussian blur (VERY IMPROTANT)   
        *   Why:
        *       - Removes sensor noise
        *       - Prevents false motion detection
        *       - Faster and more stable
        *   Grayscale -> Guassian Blur
        ***************************************************************/

       cv::GaussianBlur(
        gray,               // Input: grayscale image
        gray,               // Output: Same image overwritten (in-place)
        cv::Size(5, 5),     // 5x5 Guassian kernal
        0                   // Auto Calulate Sigma
       );

        /***************************************************************
        *   HANDLE FISRT FRAME 
        *   We need two frames to detect motion.
        *   For the first frame:
        *       - Store it as prevGray
        *       - Skip motion detction   
        ***************************************************************/

        if(firstFrame)
        {
            prevGray = gray.clone(); // Deep copy 
            firstFrame = false;
            continue; // go to next frame
        }

        /***************************************************************
        * STEP 3: Absolute Difference (Frame Differenceing)    
        * absdiff():
        *   - Computes | Current - previous |
        *   - Highlights regions where motion occurred
        * 
        * Result:
        *   - Black pixels -> No motion
        *   - Bright Pixels -> Motion
        * 
        * Compute absolute differnce between teo images
        * cv::absdiff(
        *   InputArray src1, -> first image
        *   InputArray src2, -> second image
        *   OutputArray dst  -> output diffenerence image
        * )
        ***************************************************************/

        cv::absdiff(
            gray,           // Current grayscale frame
            prevGray,       // Pervious garyscale frame
            diff            // Output image showing motion areas
        );

        /***************************************************************
        * STEP 4: Thershold the difference image
        * Threshold value: 25
        * 
        * Meaning:
        *   - Pixel difference > 25 -> Motion
        *   - Pixel difference <= 25 -> Noise
        * 
        * Output:
        *   - binary image (0 or 255)
        ***************************************************************/

        cv::threshold(
            diff,               // Input: Absolute differnence image
            binary,             // Output: Binary motion mask 
            30,                 // Threshold value (Motion sensitivity)
            255,                // Value for detected motion pixels
            cv::THRESH_BINARY   // Binary thresholding
        );

        /***************************************************************
        *   STEP 5: Count motion pixels
        * 
        * countNonZero():
        *   - Count pixels with value != 0
        *   - In binary image, this equals number of motion pixels 
        ***************************************************************/

        int motionPixels = cv::countNonZero(binary);

        /***************************************************************
        * STEP 6: Decide safety status
        * 
        * motionPixels > 5000 means:
        *   - Large area changed suddenly
        *   - Possible obstacle / human / collision risk
        ***************************************************************/

        std::string status;

        if(motionPixels > 10000)
        {
            status = "EMERGENCY STOP";
        }
        else
        {
            status = "SAFE";
        }

        /***************************************************************
        * STEP 7: Display status on video frame
        ***************************************************************/

        cv::putText(
            frame,                      // Image to draw on
            status,                     // Text
            cv::Point(50, 50),          // Position
            cv::FONT_HERSHEY_SIMPLEX,   // Font
            1.0,                        // Font Size
            cv::Scalar(0, 0, 255),      // Red Color (BGR)
            2                           // Thickness
        );

        /***************************************************************
        * DISPLAY WINDOWS
        ***************************************************************/

        cv::imshow("Camera", frame);    // Original view with status
        cv::imshow("Motion Mask", binary);  // White = motion

        
        /***************************************************************
        * UPDATE PREVIOUS FRAME
        * Must clone:
        *   - because gray will be overwritten in next loop
        ***************************************************************/

        prevGray = gray.clone();

        /***************************************************************
        * EXIT CONDITION
        ***************************************************************/

        if(cv::waitKey(1) == 27)
        {
            break; // ESC KEY
        }

    }

    return (EXIT_SUCCESS);
}