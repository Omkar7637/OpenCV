/******************************************************************************
*  File Name   : camera_capture.cpp
*  Description : Real-time CSI camera capture using OpenCV and GStreamer.
*                This application captures frames from the Jetson Nano CSI
*                camera, processes them using OpenCV, and displays the output.
*
*  Author      : Omkar Ankush Kashid
*  Created On  : 19-12-2025
*  Last Update : 29-12-2025
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

    std::cout << " \n----- IGV::Camera Pipeline Working -----" << std::endl;

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

    std::cout << " \n----- IGV::Camera Access Working -----" << std::endl;

    std::cout << "===================================================" << std::endl;
    std::cout << "\n----- Pipeline -----" << std::endl;
    std::cout << "CSI Camera Sensor\n       |       \nnvarguscamerasrc\n       |       \nNVMM (GPU Memory)\n       |       \nnvvidconv (HW Accelearted)\n       |       \n      BGRx\n       |       \nvideoconvertor (CPU)\n       |       \n    appsink\n       |       \nOpenCV cv::Mat\n" << std::endl;
    std::cout << "===================================================" << std::endl;


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

    cv::namedWindow("Camera", cv::WINDOW_NORMAL);
    cv::setWindowProperty("Camera", cv::WND_PROP_FULLSCREEN, cv::WINDOW_FULLSCREEN);

    std::cout << " \n----- IGV::Fullscreen Window Created -----" << std::endl;


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

    std::cout << " \n----- IGV::Kernal created -----" << std::endl;


    //------------------------------------------------------------------------

    std::cout << " \n----- IGV::Entering While Loop -----" << std::endl;


    // While loop for video each frame scanning
    while(true)
    {
        // Take Single frame
        cap >> frame;
        if(frame.empty())
        {
            std::cerr << "Empty frame received!" << std::endl;
            continue;
        }

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
        //      InputArray  kernel      -> structuring element (kernel)
        // )
        cv::morphologyEx(
            binary,                     // Input: binary image (0 to 255)
            morph,                      // Output: morphologically processed image
            cv::MORPH_CLOSE,            // Operation: Closing (Dilation -> Erosion)
            kernel                      // Structuring element (defines nighbothood)
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
        std::vector<std::vector<cv::Point>> countours;

        // ================= COUNTOUR DETECTION ==============================
        //
        // cv::findCountours(
        // InputOutputArray image,          -> binary image (will be MODIFIED)
        // OutputArrayOfArrays countours,   -> detected contours
        // int mode,                        -> contour retrival mode
        // int method,                      -> contour approximation method
        // )
        cv::findContours(
            morph,                  // Input binary image (after morphology)
            countours,              // Output vector containing countours
            cv::RETR_EXTERNAL,      // Retrive ONLY outermost countours
            cv::CHAIN_APPROX_SIMPLE // Compress countour points 
        );

        // Pipeline until context 
        // frame → gray → binary → morph → contours

        // Index of the largest contour found so far
        //
        // Intialized to -1 meaning 
        // - No contour selected yet
        // - Safe "Invalid" index
        int largeIndex = -1;

        // Stores the maximum area found among all countours
        //
        // Type: double
        // Reason:
        // - CountorArea() returns double
        // - Areas can be large (thousands of pixels)
        double maxArea = 0;

        // Loop thorough all detected countours
        // 
        // size_t:
        // - unsigned all integer type
        // - correct type for indexing containers returned by .size()
        for(size_t i = 0; i < countours.size(); i++)
        {
            // Calculate area of the i-th countor
            //
            // countourArea():
            // - computes area using Green's theorem
            // - Returns area in pixel units
            // - Larger objects -> larger area
            double area = cv::contourArea(countours[i]);

            // Check if this countour is larger is larger than al preivous ones
            if(area > maxArea)
            {
                // Upodate maximum area
                maxArea = area;

                // Stores index of the largest contour
                largeIndex = i;
            }
        }
        // Proceed only if a valid largest contour was found
        //
        // largeIndex == -1 means:
        // - No countours detected 
        // - Or all countours were filtered out
        if(largeIndex != -1)
        {
            // ========================= DRAW CONTOUR ============================
            //
            // cv::drawCountours(
            //      image,          -> image on which to draw 
            //      contours,       -> all detected countours
            //      contourIdx,     -> index of countor to draw
            //      color,          -> BGR color
            //      thickness       -> line thickness
            // )
            cv::drawContours(
                frame,              // Destimation image
                countours,          // All countours
                largeIndex,         // Only the largest countor
                cv::Scalar(0, 255, 0), // Green Color (B, G, R)
                2                   // Line thickness
            );

            // ===================== BOUNDING RECTANGLE ===========================
            //
            // cv::boundingRect():
            // - Computes the small upright rectangle
            // - That completely encloses the countour
            //
            // Returns:
            // - cv::Rect (x, y, windth, height)
            cv::Rect box = cv:: boundingRect(countours[largeIndex]);

            // Draw rectangle around the detected object
            //
            // cv::rectangle(
            //      image,      -> destiantion image
            //      rect,       -> rectnagle position and size
            //      color,      -> BGR color
            //      thickness   -> border thickness
            // )
            cv::rectangle(
                frame, 
                box, 
                cv::Scalar(255, 0, 0), // Blue rectangle
                2
            );

            // ===================== MOVEMENT CALCULATIONS ==========================
            //
            // cv::movements():
            // - Computes spatial movements of the countour
            // - Used to calculate cnetroid, area, orientation
            //
            // m.m00 -> Zerooth moment (area)
            // m.m10 -> First moment (x-direction)
            // m.m01 -> First moment (y-direction)
            cv::Moments m = cv:: moments(countours[largeIndex]);

            // Check to avoid division by zero
            //
            // If m.m00 == 0:
            // - Contour area is zero
            // - Centroid is undefined
            if(m.m00 != 0)
            {
                // ======================= CENTROID CALCULATIONS ========================
                //
                // Centroid formula:
                // cx = m10 / m00
                // cy = m01 / m00
                //
                // This gives the center of mass of the object
                int cx = static_cast<int>(m.m10 / m.m00);
                int cy = static_cast<int>(m.m01 / m.m00);

                // ======================= DRAW CENTROID ===============================
                // 
                // cv::circle(
                //      image,          -> destination image
                //      center,         -> center point
                //      radius,         -> circle radius
                //      color,          -> BGR color
                //      thickness       -> -1 means filled circle
                // )
                cv::circle(
                    frame,              
                    cv::Point(cx, cy),  // centroid position
                    5,                  // Radius
                    cv::Scalar(0, 0, 255), // Red Color 
                    -1                  // Filled circle
                );
            }
        }

        // =========================== DISPLAY OUTPUT ===================================
        //
        // Shows final processed frame with:
        // - countour
        // - bounding box
        // - centroid
        cv::imshow("Camera", frame);

        // =========================== EXIT CONDITION ===================================
        // 
        // cv::waitKey(1):
        // - waits 1 milliscond
        // - Allows window refresh
        // - Returns ASCII value of pressed key
        // 27 = ESC key
        if(cv::waitKey(1) == 27) break;
    }

    std::cout << " \n----- IGV::Exiting While Loop -----" << std::endl;


    
    
    std::cout << " \n----- IGV::Exiting Code -----" << std::endl;

    return 0;
}

/******************************* END OF FILE *********************************/
