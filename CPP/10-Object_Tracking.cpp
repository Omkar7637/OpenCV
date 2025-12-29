/******************************************************************************
 *  File Name   : camera_capture.cpp
 *  Description : Real-time CSI camera capture using OpenCV and GStreamer.
 *                This application captures frames from the Jetson Nano CSI
 *                camera, processes them using OpenCV, and displays the output.
 *
 *  Author      : Omkar Ankush Kashid
 *  Created On  : 19-12-2025
 *  Last Update : 19-12-2025
 *
 *  Platform    : NVIDIA Jetson Nano Super 8GB
 *  OS          : Ubuntu (JetPack)
 *  Language    : C++
 *  Framework   : OpenCV 4.x, GStreamer
 *
 *  Version     : 1.0.0
 *
 *  Notes       :
 *  - Uses nvarguscamerasrc for CSI camera access.
 *  - Requires OpenCV built with GStreamer support.
 *  - Designed for embedded vision applications.
 *
 ******************************************************************************/

// ===================== Header Files ===========================================
// 
// This is the main OpenCV header.
// it Includes almost all commenly used OpenCV modiules:
// core, imageproc, highgui, video, etc.
#include<opencv2/opencv.hpp>

// Standered c++ input-output stram library
// Used here for debugging or printing messages if nedded.
#include<iostream>

//======================= MAIN FUNCTION =========================================

int main()
{
    //====================== CAMERA ACCESS USING GStreamer ======================
    // 
    // cv:: VideoCapture can accept a GStreamser piprline string insted of a device index.
    // This is REQUIRED on NVIDIA Jetson boards to access the CSI camera efficiently
    // using the NVIDIA Argus camera stack
    cv::VideoCapture cap(
        // --------------------GStreamer Pipline String---------------------------
        //
        // nvarguscamerasrc
        //  - NVIDIA Argus Camera Source
        //  - Used ONLY for CSI camera on jetson (IMX219, IMX477, etc.)
        //  - Provide zero-copy access to camera frames via NVMM memory
        //
        // Video/x-raw(memory:NVMM)
        //  - Spacifics raw video frames
        //  - memory:NVMM means frames are stored in NVIDIA GPU Memory
        //
        // width=1920, height=1080
        //  - Resolution of the camera capture
        //  - Must match one of the camera's supported sensor modes
        //
        // Framerate=60/1
        //  - Frames per second (60 FPS)
        //  - Format is numerator/denominator
        //
        "nvarguscamerasrc !" // Direct Interface to Nvidia driver | Uses ISP (Image Signal Processor) | Support: Auto exposure/Auto while balance/HDR (If supported)
        "video/x-raw(memory:NVMM)," // NVIDIA Multimedia memory | Keeps frames in GPU memory | Avoides unnecssary memory Copies | Critical for: High FPS/Low Latency/ AI inference pipelines  
        "width=1920, height=1080, framerate=60/1 ! "

        // nvvidconv
        //  - NVIDIA hardware-acclerated video converter
        //  - Converts from NVMM memory format to understand system memory
        //  - Also handles color format conversion
        "nvvidconv !" // Uses jetson hardware | Much faseter than videconvert | Converts: memory layout/Color formats/Resolution 

        // Video/x-raw, format=BGRx
        //  - Intermediate format
        //  - BGRx = BGR + unused alpha channel
        //  - Required because videoconvert works efficently with BGRx
        "video/x-raw, format=BGRx !" 

        // videoconverter
        //  - CPU-based colro format converter
        //  - Converts BGRx -> BGR
        //  - OpenCV expects BGR format
        " videoconvert !"

        // Video/x-raw, format=BGR
        //  - Final format passed to OpenCV
        //  - 3-channel BGR image (CV_8UC3)
        " video/x-raw, format=BGR !"

        // appsink
        //  - Sink elemnt that allows applications (OpenCV) to read frames
        //  - Without appsink, OpenCV cannot access the video stream
        " appsink",

        // --------------------------Backend Selection
        //
        //cv::CAP_GSTREAMER
        //  - Force OpenCV to use the GStreamer backend
        //  - Mandatory when passing a GStreamer pipeline string
        cv::CAP_GSTREAMER
    );

    // ================== CAMERA ACCESS CHECK ==========================================
    //
    //  - GStreamer pipeline creation
    //  - Camera availability
    //  - Successful negotiation of caps (resolutionn, FPS, format)
    // 
    // If any element in the ipeline fails, this returns false
    if(!cap.isOpened())
    {
        std::cout << "Camera Not supported!" << std::endl;
        return -1; // Exit program safely
    }

    // =============== Pipeline Breakdown==================
    //
    //              CSI Camera Sensor
    //                      |
    //              nvarguscamerasrc
    //                      |
    //              NVMM (GPU Memory)
    //                      |
    //              nvvidconv (HW accelerated)
    //                      |
    //                     BGRx
    //                      |
    //              videoconverter (CPU)
    //                      |
    //                     BGR
    //                      |
    //                   appsink
    //                      |
    //              OpenCV cv::Mat
    //
    //====================================================

    // OpenCV Variables
    cv::Mat frame, gray, binary, morph;

    //--------------------------------------------------------------------------

    // Create a structuring element (also called a kernel) for morphology
    //
    // cv::Mat kernel
    //  - cv::Mat is OpenCV's matrix class
    //  - Here it stores a small 2D binary mask used in morphological operations
    //
    // cv::getStructuringElement()
    //  - OpenCV function to generate predefined kernel shapes
    //
    // cv::MORPH_RECT
    //  - Shape of the kernel
    //  - MORPH_RECT  -> Rectangular kernel (all ones)
    //  - Other options:
    //      cv::MORPH_ELLIPSE -> Oval / circular shape
    //      cv::MORPH_CROSS   -> Cross (+) shape
    //
    // cv::Size(5, 5)
    //  - Kernel width  = 5 pixels
    //  - Kernel height = 5 pixels
    //  - Must usually be odd numbers (3,5,7...) so there is a center pixel
    //
    // Result:
    //  - A 5x5 matrix filled with 1s (for MORPH_RECT)
    cv::Mat kernel = cv::getStructuringElement(
        cv::MORPH_RECT,
        cv::Size(5, 5)
    );

    //------------------------------------------------------------------------


    // While loop for video each frame scanning
    while(true)
    {
        // Take Single frame
        cap >> frame;

        // Convert a color image (BGR format) into a grayscale
        //
        // cv::cvtColor(
        //  InputArray scr,     -> source Image (Color Image)
        //  OutputArray dst,    -> Destination image (Grayscale image)
        //  int code            -> Color conversion Mode
        // )

        cv::cvtColor(
            frame, // -> Input: Origianl BGR image from camera
            gray, // Output: Single-channel (Grayscale image)
            cv::COLOR_BGR2GRAY // Conversion type: BGR -> Grayscale
        ); 

        //----------------------------------------------------------------

        // Apply thersholding to convert a grayscale image a binary image
        //
        // cv::thershold(
        //      InputArray  src,        -> Source Image (Grayscale only)
        //      OutputArray dst,        -> Output binary Image
        //      double      thresh,     -> tehrshold value (Ignore in OTSU)
        //      double      maxVal,     -> value assigned to pixel passing the thershold
        //      int         type        -> thersholding method
        // )
        //
        // This convertes a grayscale image into only TWO Values:
        // - 0      (Black)
        // - 255    (White)
        cv::threshold(
            gray,       // Input Grayscale image
            binary,     // Output binary image
            0,          // Thershold value (not used by OTSU)
            255,        // Maximum value for THRESH_BINARY
            cv::THRESH_BINARY | // Binary thersholding
            cv::THRESH_OTSU // Automatically calculates best thershold
        );

        //-------------------------------------------------------------------

        // Perform a morphological opertaion on a binary image
        //
        // cv::morphologyEx(
        //      InputArray  src,        -> source image (binary image)
        //      OutputArray dst,        -> output image after morphology
        //      int         op,         -> type of morphological operations
        //      InputArray  kernal      -> structuring element (kernal)
        // )
        cv::morphologyEx(
            binary,                     // Input: binary image (0 to 255)
            morph,                      // Output: morphologically processed image
            cv::MORPH_CLOSE,            // Operation: Closing (Dilation -> Erosion)
            kernal                      // Structuring element (defines nighbothood)
        );

        // ================= CONTOUR STORAGE =================================
        // std::vector<std::vector<cv::Point>> contours
        //
        // Meaning:
        // - Outer vector -> holds ALL detected countours
        // - Inner vector -> holds points of ONE countour
        // - cv::Point    -> repersents a 2D pixel coordinates (x, y)
        //
        // Each countour is a basically a list of connected boundary pixel
        std::vector<std::vetor<cv::Point>> countours;

        // ================= COUNTOUR DETECTION ==============================
        //
        // cv::findCountours(
        // InputOutputArray image,          -> binary image (will be MODIFIED)
        // OutputArrayOfArrays countours,   -> detected contours
        // int mode,                        -> contour retrival mode
        // int method,                      -> contour approximation method
        // )
        cv::findcountours(
            morph,                  // Input binary image (after morphology)
            countours,              // Output vector containing countours
            cv::RETR_EXTERNAL,      // Retrive ONLY outermost countours
            cv::CHAIN_APPROX_SIMPLE // Compress countour points 
        );

        // Pipeline until contexte



    }

    
    

    return 0;
}

/******************************* END OF FILE *********************************/
