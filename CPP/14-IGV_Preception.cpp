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
 *      - Uses nvarguscamerasrc for CSI
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
    // ===================== CAMERA INTIALIZATION ====================
    
    std::cout << std::endl << "========== Camera Intitializations ==========" << std::endl;
    cv::VideoCapture cap(
        "nvarguscamerasrc !"                // Direct interface to nividia hardware (Mostly auto setting)
        "video/x-raw(memory:NVMM), "        // NVIDIA Multimedia memory
        "width=1280, height=720, frame=60/1 !"
        "nvvidconv !"                        // Uses Jetson hardware | Much faster for video converter
        "video/x-raw, format=BGRx !"        //  BGR + usused alpha channel required | Video Converter work efficently with BGRx
        "videoconvert !"                    // CPU-Based color format converter | Convert BGRx -> BGR | OpenCV expects BGR format
        "video/x-raw, format=BGR !"         // Final format passed to OpenCV | 3-channel BGR image (CV_8UC3)
        "appsink ",                         // sink element that allows application (OpenCV) to read frames | withour appsink, OpenCV cannot access the videostream

        // BACKEND SELECTION
        cv::CAP_GSTREAMER // Force OpenCV to Use the GStreamer backend | mandatory when passing a GStreamer pipline string
    );

    if(!cap.isOpened()) // if any element in the pipeline fails, this return false
    {
        std::cout << "========== Camera Not Suported ==========" << std::endl;
        return(EXIT_FAILURE); // Exit program safely
    }

    // ========== IMAGE MATRICES (PIPLINE STAGES) ==========
    cv::Mat frame;          // Original BGR frame from camera
    cv::Mat gray;           // Graysacle version of frame 
    cv::Mat prevGray;       // Privous grayscale frame (for motion detection)

    cv::Mat binary;         // Binary image for path detction
    cv::Mat roi;            // Region of interest for path detection
    cv::Mat diff;           // Absolute difference between frames
    cv::Mat motionMask;     // Binary mask highlighting motion areas
    
    // Flag to handle the first frame safely
    bool firstFrame = true;

    // ========== MAIN PROCESSING LOOP ==========
    while(true)
    {
        // Capture New Frame
        // Check if the frame is empty
        if(!cap.read(frame) || frame.empty())
        {
            std::cerr << "ERROR:Empty Frame recived!" << std::endl;
            continue;
        }

        /******************************************************
         * STEP 1. PREPROCESSING
         *     - Convert to grayscale
         *      - Apply Gaussian blur to reduce noise
        *******************************************************/

        // Convert BGR image to single-channel grayscale
        cv::cvtColor(
            frame,              // Input: Original BGR image from camera
            gray,               // Output: Single-channel (Grayscale image)
            cv::COLOR_BGR2GRAY  // Conversion type: BGR -> Grayscale
        );

        // Apply Guassian Blur (5x5 kernal)
        // Purpose: 
        //      - Reduce camera noise
        //      - Improve threshold stability
        //      - Prevent flase motion dection
        cv::GaussianBlur(
            gray,               // Input: grayscale image
            gray,               // Output: Same image overwritten (in-place)
            cv::Size(5, 5),     // 5x5 Guassian kernal
            0                   // Auto calculate sigma
        );

        if (firstFrame)
        {
            prevGray = gray.clone();
            firstFrame = false;
            continue;
        }

        /******************************************************
         * STEP 2. EMERGENCY STOP (MOTION DETECTION)
         *      - Detect sudden scene changes
         *      - Trigger emergency stop if motion is large
        ******************************************************/

        bool emergencyStop = false;

        if(!firstFrame)
        {
            // Compute absolute pixel-wise difference
            // Highlight moving regions between frames
            cv::absdiff(
                gray,           // Current grayscale frame
                prevGray,       // Previous grayscale frame 
                diff            // Output image showing motion areas
            );

            // Threshold differnce image to create motion mask
            // diff > 25 -> motion pixel (255)
            cv::threshold(
                diff,           // Input: Absolute differnence image
                motionMask,         // Output: motion mask
                25,             // Threshold value (motion sesitivity)
                255,            // Value for detected motion pixels
                cv::THRESH_BINARY // Binary thresholding
            );

            // Count motion pixels 
            int motionPixels = cv::countNonZero(motionMask);

            // If motion exceeds safe threshold -> EMERGENCY STOP
            if(motionPixels > 5000)
            {
                emergencyStop = true;
            }
        }

        // Store current frame as previous for next iteration
        // clone() is mandatory to avoid memory overwrite
        prevGray = gray.clone();

        /******************************************************
         * STEP 3. PATH DETECTION (ONLY IF SAFE)
         *      - Convert image to binary using OTSU
         *      - Extract bottom-half ROI
         *      - Divide ROI into 3 zones
         *      - Decide movement direction
        ******************************************************/

        std::string direction = "STOP";

        if(!emergencyStop)
        {
            // Apply OTSU thresholding for automatic segmentation
            cv::threshold(
                gray,           // Input: Grayscale Image
                binary,         // Output: binary image (0 to 255)
                0,              // Dummy thershold (OTSU calculates it)
                255,            // Foregriund pixel value (white)
                cv::THRESH_BINARY | // Binary therhsolding
                cv::THRESH_OTSU // Automatic threshold selection
            );

            // IMAGE DIMENTIONS
            int h = binary.rows;
            int w = binary.cols;

            if (binary.empty()) continue;

            // Define ROI as botton half of the image
            // Reason:
            //      - Path exists near robot, not in sky/background
            // Take bottom 70% of the frame as ROI
            int roiStartY = h * 0.3;      // Start at 30% height
            int roiHeight = h * 0.7;      // Cover 70% of image

            roi = binary(
                cv::Rect(
                    0,          // Start from left edge
                    roiStartY,  // Start from middle of the image vertically
                    w,          // Use full image width
                    roiHeight   // Bottom half of the image
                )
            );

            if (roi.empty()) continue;
            
            // Divide ROI into three equal vertical zones
            int zoneWidth = w / 3;

            // These zones represents possible navigation directions:
            // [ LEFT ZONE ] [ CENTER ZONE ] [ RIGHT ZONE ]
            // The Robot decides direction based on where the path
            // (White pixels) is strongest.
            cv::Mat left = roi(
                cv::Rect(
                    0,          // x start
                    0,          // y start
                    zoneWidth,  // Zone width
                    roi.rows    // fill ROI height
                )
            );

            cv::Mat center = roi(
                cv::Rect(
                    zoneWidth,  // x start
                    0,          // y start
                    zoneWidth,  // Zone Width
                    roi.rows    // full ROI height
                )
            );

            cv::Mat right = roi(
                cv::Rect(
                    2 * zoneWidth, // x start
                    0,          // y start
                    zoneWidth,  // Zone Width
                    roi.rows    // full ROI height
                )
            );

            // Count path pixels in each zones
            int leftCount = cv::countNonZero(left);
            int centerCount = cv::countNonZero(center);
            int rightCount = cv::countNonZero(right);

            // Direction decision logic
            // Priority given to forward movement for stability
            if(centerCount >= leftCount && centerCount >= rightCount)
            {
                direction = "FORWORD";
            }
            else if(leftCount > rightCount)
            {
                direction = "LEFT";
            }
            else
            {
                direction = "RIGHT";
            }

        }

        /******************************************************
         * STEP 4. DISPLAY & VISUALIZATION
        ******************************************************/

        if(emergencyStop)
        {
            // Display emergency warning in RED
            cv::putText(
                frame,                      // Image to draw on
                "EMERGENCY STOP",           // Text
                cv::Point(50, 100),          // Positions
                cv::FONT_HERSHEY_SIMPLEX,   // Font
                3,                          // Font Size
                cv::Scalar(0, 0, 255),      // RED Color (BGR)
                2                           // Thickness
            );
        }
        else
        {
            // Dispay navigation decision in GREEN
            cv::putText(
                frame,                      // Image to draw on
                direction,                  // Text
                cv::Point(50, 100),          // Position
                cv::FONT_HERSHEY_SIMPLEX,   // FONT
                3,                          // Font Size
                cv::Scalar(0, 255, 0),      // Green Color
                2                           // Thickness
            );
        }

        // Shoe Live Camera feed
        cv::imshow("IGV Camera", frame);

        // show region used for path decision
        cv::imshow("ROI", roi);

        // Show motion detection mask
        cv::imshow("Morion Mask", motionMask);

        // EXIT CONDITION

        if(cv::waitKey(1) == 27)
        {
            break; // ESC Key
        }
    }

    return(EXIT_SUCCESS);
}