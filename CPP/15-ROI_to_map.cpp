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
const int MAP_ROWS = 9*3;
const int MAP_COLS = 16*3;

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
            gray,
            cv::COLOR_BGR2GRAY
        );

        /*
            Apply guassian blur
            Purpose:
                - Reduce camera noise
                - Improve thresholding stability
        */
       
        cv::GaussianBlur(
            gray,
            gray, 
            cv::Size(5, 5), 
            0
        );

        /*
            Convert grayscale image to binary using OTSU
            Purpose:    
                - Automatically separate forground and background
        */

        cv::threshold(
            gray, 
            binary, 
            0, 
            255, 
            cv::THRESH_BINARY | cv::THRESH_OTSU
        );

        // ========== ROI SECTION ==========
        // Image dimentions
        int h = binary.rows;
        int w = binary.cols;

        if(binary.empty()) continue;

        /* 
            Define ROI as the bottom Image
            Resson:
                - Obstacles and free path are close to the robot
                - Upper half usually contains irrelevent information
        */

        int roiStartY = h * 0.3; // Start at 30% image
        int roiHeight = h * 0.7; // Cover 70% of image

        roi = binary(
            cv::Rect(
                0,          
                roiStartY,
                w, 
                roiHeight  
            )
        );

        if(roi.empty()) continue; 

        // ========== GRID CELL SIZE CALCULATION ==========
        // Convert ROI into MAP_ROWS x MAP_COLS grid
        int cellwidth = roi.cols / MAP_COLS;
        int cellHeight = roi.rows / MAP_ROWS;
        
        // ========== OCCUPANCY MAP UPDATE ==========
        // Loop over each cell in the occupancy grid
        for(int r = 0; r < MAP_ROWS; r++)
        {
            for (int c = 0; c < MAP_COLS; c++)
            {
                // Compute pixel coordinates of the cell
                int x = c * cellwidth;
                int y = r * cellHeight;

                // Extract cell ROI
                cv::Rect cellRect(x, y, cellwidth, cellHeight);
                cv::Mat cell = roi(cellRect);

                // Count number of white pixels int he cell
                int whitePixels = cv::countNonZero(cell);

                // Total number of pixels in the cell
                int totalPixels = cell.rows * cell.cols;

                // Classification logic:
                // If majority of pixels are white -> free space
                // otherwise -> obstacle
                if(whitePixels > totalPixels / 2)
                {
                    occupancyMap[r][c] = 1; // free
                }
                else
                {
                    occupancyMap[r][c] = 2; // obstacle
                }
            }
        }

        // ========== OCCUPANCY MAP VISUALIZATION ==========
        // Create visualization image
        // Each grid cell is drawn as a 30x30 square
        cv::Mat mapVis(MAP_ROWS * 30, MAP_COLS * 30, CV_8UC3);

        for(int r = 0; r < MAP_ROWS; r++)
        {
            for(int c = 0; c < MAP_COLS; c++)
            {
                cv::Scalar color;

                // Assign color based on occupancy state
                if(occupancyMap[r][c] == 0)
                {
                    color = cv::Scalar(128, 128, 128); // unkonn (gray)
                }
                else if(occupancyMap[r][c] == 1)
                {
                    color = cv::Scalar(255, 255, 255); // Free (White)
                }
                else
                {
                    color = cv::Scalar(0, 0, 0); // obstacle (black)
                }

                // Draw filled rectangle for each grid cell
                cv::rectangle(
                    mapVis, 
                    cv::Rect(c * 30, r * 30, 30, 30), 
                    color, 
                    cv::FILLED
                );
            }
        }

        // ========== DISPLAY OUTPUTS ==========
        cv::imshow("camera", frame);    // Origianl camera feed
        cv::imshow("ROI", roi);         // Bottom-half region
        cv::imshow("occupancy Map", mapVis); // Grid-based map

        // ========== EXIT CONDITION ==========
        if(cv::waitKey(1) == 27) // ESC Key
        {
            break;
        }
        //// END OF WHILE LOOP ////
    }
    return(EXIT_SUCCESS);
} 