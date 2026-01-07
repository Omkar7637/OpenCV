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
 *      - Uses nearguscamerasrc for CSI
 *      - Requires OpenCV built with GStreamer support.
 *      - Designed for embedded vision applications.
*****************************************************************************************/