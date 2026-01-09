/*****************************************************************************************
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
*****************************************************************************************/