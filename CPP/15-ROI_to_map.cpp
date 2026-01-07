/******************************************************************************************
 * File Name    : 15-ROI_to_ma.cpp
 * Project      : IGV Vision System - Path Detection + Emergency Stop
 * Language     : C++
 * Library      : OpenCV
 * 
 * Desccription : 
 *  - Capture Live Camera feed
 *  - Converts image into a binary representation
 *  - Extaracts bottom-half Region of Interest (ROI)
 *  - Divides ROI into a grid (occupancy map) 
 *  - Classifies each grid cell as:
 *          0 = Unknown
 *          1 = Free
 *          2 = Obstacle
 *  - Visualizes the occupancy grid
 * 
 * Author       : Omkar Ankush Kashid
 * Created on   : 07-01-2026
 * Last Uodated : 07-01-2026
 * Platform     : NIVIDIA JETSON NANO SUPER 8GB
 * OS           : Ubuntu (jetpack)
 * Framework    : OpenCV 4.x, GStreamer
 * Version      : 1.0.15
 * Notes        : 
 *  - Uses nvarguscamerasrc for CSI
 *  - Requires OpenCV built with GStreamer support
 *  - Designed for embedde vision applications. 
******************************************************************************************/

// ============================== HEADER FILES ==============================
#include<opencv2/opencv.hpp>
#include<iostream>
#include<cstdlib>

// ========== OCCUPANCY MAP CONFIGRATION ==========
// Number of rows and columns in the occupancy grid
// This defines the resolution of the map
const int MAP_ROWS = 10;
const int MAP_COLS = 20;

// Occupancy map storage
// 0 = unkown
// 1 = free space
// 2 = obstacle
int occupancyMap[MAP_ROWS][MAP_COLS] = {0};

// =========================== ENTRY POINT FUNTION ==========================
int main()
{
    // ==================== CAMERA INTIALIZATION ====================
    std::cout << std::endl << "========== CAMERA INTIALIZATIONS ==========" << std::endl;
    
    cv::VideoCapture cap(
        "nvarguscamerasrc !"
        "video/x-raw(memory:NVMM), "
        "width=1280, height=720, frame=60/1 !"
        "nvvidconv !"
        "video/x-raw, format=BGRx !"
        "videoconvert !"
        "video/x-raw, format=BGR !"
        "appsink",
        cv::CAP_GSTREAMER
    );

    if(!cap.isOpened())
    {
        std::cout << "========== CAMERA NOT SUPPORTED ==========" << std::endl;
        return(EXIT_FAILURE);
    }

    // ==================== IMAGE MATRICES (PROCESSING STAGES) ====================
    cv::Mat frame;  // Original camera frame (BGR)
    cv::Mat gray;   // Grayscale image
    cv::Mat binary; // Binary image
    cv::Mat roi;    // Region of Interest (bottom half)

    // ==================== MAIN LOOP ====================
    while(true)
    {
        if(!cap.read(frame) || frame.empty())
        {
            std::cerr << "ERROR:Empty Frame recived!" << std::endl;
            continue;
        }

        /* ========== PREPROCESSING ==========
            Convert BGR image to grayscale
            Purpose: 
                - Reduce data from 3 channels to 1
                - Simplify further processing
        */
        cv::cvtColor(
            frame, 
        )

        //// END OF WHILE LOOP ////
    }
} 