/*****************************************************************************************
 * Author       : Omkar Ankush Kashid
 * Created on   : 09-01-2026
 * Last updated : 09-01-2026
 * Platform     : NVIDIA JETSON NANO SUPER 8GB
 * OS           : Ubuntu (jetpack)
 * Framework    : OpenCV 4.x, GStreamer
 * Version      : 1.0.16
 * File Name    : 16-Persistent_map.cpp
 * Project      : IGV Vision System - Path Detection
 * Language     : C++
 * Library      : OpenCV
 * 
 * Description  : 
 *      - Capture live camera feed
 *      - Convertes image to binary using OTSU thresholding
 *      - Extracts bottom-half Region of Interest (ROI)
 *      - Divides ROI into a fixed grid (MAP_ROWS x MAP_COLS)
 *      - Classifies each grid cell as FREE or OBSTACLE
 *      - Maintains a PERSISTENT occupancy map across frames
 * 
 * Occupancy States:
 *      - 0 : Unknown (never observed)
 *      - 1 : Free space
 *      - 2 : Obstacle (Persistent once detected)
 * 
 * Application  :
 *      - IGV / AGV navigation
 *      - Local obstacle mapping
 *      - Path planning input (A*, Dijkstra, etc.)
 * 
 * Notes        :
 *      - Uses nvarguscamerasrc for CSI
 *      - Requires OpenCV built with GStreamer support
 *      - Designed for embedded vision applications.  
*****************************************************************************************/

// ============================== HEADER FILES ==============================
#include<opencv2/opencv.hpp>
#include<iostream>
#include<cstdlib>

// ========== OCCUPANCY GRID CONFIGRATION ==========
// Grid resolution (map size)
const int MAP_ROWS = 10;
const int MAP_COLS = 20; 

// Presistent occupancy grid
// Initialized to unkonwn (0)
int occupancyMap[MAP_ROWS][MAP_COLS] = {0};

int main()
{
    // ==================== CAMERA INITIALIZATIONS ====================
    // Open default camera (index 0)
    cv::VideoCapture cap(
        "cvarguscamerasrc !"
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
        std::cout << "========== CAMERA NOT SOPPORTED ==========" << std::endl;
        return(EXIT_FAILURE);
    }

    // ==================== IMAGE CONTAINERS ====================
    cv::Mat frame;  // Original BGR camera frame 
    cv::Mat gray;   // Grayscale image
    cv::Mat binary; // Binary image after thresholding 
    cv::Mat roi;    // Bottom-hlaf Region of Interest

    // ==================== MAIN PROCESSING LOOP ====================
    while(true)
    {
        // Capture frame | Stope execution from the camera
        if(!cap.read(frame) || frame.empty())
        {
            std::cerr << "ERROR:Empty Frame Recived!" <<std::endl;
        }

        // ========== PREPROCESSING ==========
        // Convert BGR image to grayscale
        // Reduce data size and simplifies processing
        cv::cvtColor(
            frame, 
            gray, 
            cv::COLOR_BGR2GRAY
        );

        // Apply Gussian blur
        // Purpuise:
        //      - Reduce sensor noise
        //      - Stablize thresholding
        cv::GaussianBlur(
            gray, 
            gray, 
            cv::Size(5, 5), 
            0
        );

        // Apply OTSU thresholding
        // Automatically seperated foreground and bakground
        cv::threshold(
            gray, 
            bin
        )
    }
    return(EXIT_SUCCESS);
}