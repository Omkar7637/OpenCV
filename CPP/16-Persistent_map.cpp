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
            binary, 
            50,
            255, 
            cv::THRESH_BINARY | cv::THRESH_OTSU
        );

        // ========== ROI SELECTION (BOTTOM HALF) ==========
        // Image dimentions 
        int h = binary.rows;
        int w = binary.cols;

        if(binary.empty()) continue;

        // Use only the bottom hlaf of the image 
        // Reson:
        //  - Obstacle re;vent to navigation are near the rorbot
        //  - Upper half contains irrelevant beackground
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

        // ========== GRID CELL SIZE COMPUTATION ==========
        // Each ROI is divided into MAP_ROWS / MAP_COLS cells
        int cellWidth = roi.cols / MAP_COLS;
        int cellHeight = roi.rows / MAP_ROWS;

        // ========== UPDATE OCCUPANCY MAP (PERSISTENT LOGIC) ==========

        for(int r = 0; r < MAP_ROWS; r++)
        {
            for(int c = 0; c < MAP_COLS; c++)
            {
                // Compute top-left pixel of the current cell
                int x = c * cellWidth;
                int y = r * cellHeight;

                // Extract the cell as a sub_ROI (Zero-copy)
                cv::Mat cell = roi(
                    cv::Rect(
                        x, 
                        y, 
                        cellWidth, 
                        cellHeight
                    )
                );

                // Count number of whte pixels (forground)
                int whitePixels = cv::countNonZero(cell);

                // Total number of pixels in this cell
                int totalPixels = cell.rows * cell.cols;

                // observation from current frame:
                //      - Majority white -> FREE
                //      - Otherwise -> OBSTACLE
                int observedState = (whitePixels > totalPixels / 2) ? 1 : 2;

                /************************************************
                 * PERSISTENT UPDATE RULES
                 * 
                 * 1. If cell is UNKONWN -> accept observation
                 * 2. If cell was FREE but now OBSTACLE -> Update
                 * 3. If cell already OBSTACLE -> keep it
                 * 
                 * This ensures obstacles persist across frames
                *************************************************/
               
                if(occupancyMap[r][c] == 0)
                {
                    occupancyMap[r][c] = observedState;
                }
                else if(occupancyMap[r][c] == 1 && observedState == 2)
                {
                    occupancyMap[r][c] = 2;
                }
                // If already obstacle, do nothing
            }
        }

        // ========== VISIALIZATION OF OCCUPANCY MAP ==========
        // Create visualization image 
        // Each grid cell is drawn as 30x30 square

        int CELL_SIZE = 40;

        cv::Mat mapVis(
            MAP_ROWS * CELL_SIZE,
            MAP_COLS * CELL_SIZE,
            CV_8UC3
        );

        mapVis.setTo(cv::Scalar(50, 50, 50));
        // cv::Mat mapVis(MAP_ROWS * 30, MAP_COLS * 30, CV_8UC3);

        for (int r = 0; r < MAP_ROWS; r++)
        {
            for (int c = 0; c < MAP_COLS; c++)
            {
                cv::Scalar color;

                // Color Coding
                // - Gray -> unkonwn
                // - White -> Free space
                // - Black -> Obstacle
                if(occupancyMap[r][c] == 0)
                {
                    color = cv::Scalar(128, 128, 128);
                }
                else if(occupancyMap[r][c] == 1)
                {
                    color = cv::Scalar(255, 255, 255);
                }
                else
                {
                    color = cv::Scalar(0, 0, 0);
                }

                cv::Rect cell(
                    c * CELL_SIZE,
                    r * CELL_SIZE,
                    CELL_SIZE,
                    CELL_SIZE
                );

                cv::rectangle(mapVis, cell, color, cv::FILLED);
                cv::rectangle(mapVis, cell, cv::Scalar(80,80,80), 1);

                // Draw Filled rectangle for the grid cell
                // cv::rectangle(
                //     mapVis, 
                //     cv::Rect(c * 30, r * 30, 30, 30),
                //     color, 
                //     cv::FILLED
                // );
            }
        }

        // DISPLAY WINDOWS
        cv::imshow("Camera", frame);    // Original camera feed
        cv::imshow("ROI", roi);         // Processed region
        cv::imshow("Persistent Map", mapVis); // Occupancy grid

        // EXIT CONDITION
        if(cv::waitKey(1) == 27) break;// ESC key
    }

    return(EXIT_SUCCESS);
}