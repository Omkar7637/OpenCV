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
#include<opencv