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
*****************************************************************************************/