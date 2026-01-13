/************************************************************************************************************
*   File Name   : 12-Path_decision.cpp
*   Discription : Path direction decision (Left/Center/Right) Robot should go Left, straight, or right.
*                 vision + decision
*                 # Human Thinking vs Robot thinking
*                       Human: "The path is more open on right, I'll turn right."
*                       Robot: "There are more WHITE pixel on the right."
*                   So we convert intuition -> Numbers
*                 # Divide the ROI into 3 Zones
*                       | LEFT | CENTER | RIGHT |
*                       Each zone:  Count WHITE pixels
*                                   More White = More free space
*   Author      : Omkar Ankush Kashid
*   Created On  : 06-01-2026
*   Last updated: 06-01-2026
*   Platform    : NVIDIA Jetson Nano Super 8GB
*   OS          : Ubuntu (JetPack)  
*   Language    : C++
*   Framework   : OpenCV 4.x, GStreamer
*
*   Version     : 1.0.0
*
*   Notes       :
*       - Uses nvarguscamerasrc for CSI camera access.
*       - Requires OpenCV built with GStreamer support.
*       - Designed for embedded vision applications.

************************************************************************************************************/

// ==================== HEADER FILES ====================
#include <opencv2/opencv.hpp> 
#include <iostream>
#include <cstdlib> // For EXIT_SUCCESS, EXIT_FAILURE

int main()
{
    // Local Variable decalarations
    // OpenCV Related variable 
    cv::Mat frame, gray, binary, roi;

    // ========== CSI Camera Pipeline ==========
    // CSI Camera pipeline required for jetson nano
    std::cout << std::endl;
    std::cout << "----- IGV::Pipeline Intialization Start -----" << std::endl;
    cv::VideoCapture cap(
        "nvarguscamerasrc !"                    // Direct interface to nividia driver (Auto setting)
        "video/x-raw(memory:NVMM),"             // Nvidia multimedia memory
        "width=1920, height=1080, frame=60/1 !"
        "nvvidconv !"                           // use jetson hardware | Converter: Memory Layout, color formats, resolutions
        "video/x-raw, format=BGRx !"            // BGR + unused alpha channel required videoconverter works efficently with BGRx
        "videoconvert !"                        // CPU based color format converter BGRx -> BGR OpenCV expects BGR format
        "video/x-raw, format=BGR !"             // Final format passed to the OpenCV | 3-Channel BGR image (CV_8UC3)
        " appsink ",                            // Sink elements that allows application (OpenCV to read frames | Without appsink, OpenCV cannot access the Video Stream)

        // ========== Backend Selection ==========
        cv::CAP_GSTREAMER                       // Force OpenCV to use the GSTREAMER backend | Mandatory when passing a GSTREAMER pipeline string
    );

    std::cout << "----- IGV::Camera Pipeline Scessfully intialized -----" << std::endl;

    // ========== Camera Access ==========
    // 
    // - GStreamer Pipeline creation | Camera avilabilty
    // - Successful negotioantion of caps (resolution, FPS, Format)
    if(!cap.isOpened()) // If any Element in the pipeline fails, this return false
    {
        std::cout << "----- IGV::Camera Not Supported!! -----" << std::endl;
        return(EXIT_FAILURE); // Exit program safely
    }

    std::cout << "----- IGV::Camera Working -----" << std::endl;

    std::cout << "===================================================" << std::endl;
    std::cout << "\n----- IGV::Pipeline -----" << std::endl;
    std::cout << "CSI Camera Sensor\n       |       \nnvarguscamerasrc\n       |       \nNVMM (GPU Memory)\n       |       \nnvvidconv (HW Accelearted)\n       |       \n      BGRx\n       |       \nvideoconvertor (CPU)\n       |       \n    appsink\n       |       \nOpenCV cv::Mat\n" << std::endl;
    std::cout << "===================================================" << std::endl;

    // While Loop for video each frame scanning
    std::cout << "----- IGV::Entering While Loop -----" << std::endl;
    while(true)
    {
        // Take a single frame 
        // Camera -> Frame 
        cap >> frame; 

        // Check if frame is empty
        if(frame.empty())
        {
            std::cerr << "IGV::ERROR::EMPTY FRAME RECEVIED!" << std::endl;
            return(EXIT_FAILURE);
        }

        // Step 1.Garyscale
        // frame -> Grayscale
        cv::cvtColor(
            frame,              // Input: Original BGR image from camera
            gray,               // Output: Signal-cahnnel (Grayscale image)
            cv::COLOR_BGR2GRAY  // Convertion type: BGR -> Grayscale
        );

        // Step 2.Blur
        // Grayscale -> blur
        cv::GaussianBlur(
            gray,               // Input: grascale image 
            gray,               // Output: Same image overwritten (in-place)
            cv::Size(5, 5),     // 5x5 Gaussion kernal
            0                   // Auto calculate Sigma
        );

        // Step 3.Thershold
        // Gaussian blur -> Thersholding
        cv::threshold(
            gray,               // Input Grayscale Image (Blur)
            binary,             // Output Binary Image
            0,                  // Thershold value (not used by OTSU)
            255,                // Maximum Value for THRESH_BINARY
            cv::THRESH_BINARY | // Binary therhsolding
            cv::THRESH_OTSU     // Automatically calculates best thershold
        );

        // Step 4.ROI (Botton Half)
        int h = binary.rows;    // Total Height of the binary image (In Pixels)
        int w = binary.cols;    // Total Width of the binayr image (In Pixels)

        // Create a ROI Using cv::Rect()
        roi = binary(cv::Rect(
            0,                  // Start from left edge
            h/2,                // Start from middle vertically
            w,                  // Fill width
            h/2                 // Bottom half of the image  
        ));

        // Step 5. Divide ROI into 3 Zones
        // LEFT | CENTER | RIGHT

        // Width of each Zone (ROI is divide into 3 equal parts)
        int zoneWidth = w / 3;

        /*
        *   Create sub-ROIs (zones) from the main ROI
        *   Note:   
        *       - These are NOT copies
        *       - They are views into the original ROI memory
        *       - Extermely fast and memory-effiecent
        * 
        *   cv::Rect(x, y, width, height)
        */

        // Left Zone: from x = 0 to x = zoneWidth
        cv::Mat leftzone = roi(
            cv::Rect(
                0,              // x Start
                0,              // y start
                zoneWidth,      // width
                roi.rows        // Full height of ROI
            )
        );

        // Center zone: from x = zoneWidth to x = 2 * zoneWidth
        cv::Mat centerZone = roi(
            cv::Rect(
                zoneWidth,      // x Start
                0,              // y Start
                zoneWidth,      // Width
                roi.rows        // Full height of ROI
            )
        );

        // Right Zone: from x = 2 * zoneWidth to x = 3 * zoneWitdh
        cv::Mat rightZone = roi(
            cv::Rect(
                2 * zoneWidth,  // x Start
                0,              // y Start
                zoneWidth,      // Width
                roi.rows        // Full height of ROI
            )
        );

        // Step 6. Count White Pixel in each zone
        /*
        *   coutNonZero():
        *       - Counts pixel with value != 0
        *       - In binary image:
        *           0   -> Background
        *           255 -> path / line / object 
        */

        int leftCount = cv::countNonZero(leftzone);
        int centerCount = cv::countNonZero(centerZone);
        int rightCount = cv::countNonZero(rightZone);

        // Step 7. Decide movement direction
        /*
        *   Logic: 
        *       - If center has most white pixel -> FORWORD
        *       - Else if left has more than right -> LEFT
        *       - Else -> Right
        */

        std::string direction;

        if(centerCount >= leftCount && centerCount >= rightCount)
        {
            // Path is mostly in the center -> gp Straight
            direction = "FORWARD";
        }
        else if(leftCount > rightCount)
        {
            // Path is stronger on the left -> turn left
            direction = "LEFT";
        }
        else
        {
            // Path is Stronger on the right -> turn right
            direction = "RIGHT";
        }

        // Step 8. Display the decision on the frame
        cv::putText(
            frame,                      // Image to draw text on
            direction,                  // Text string 
            cv::Point(50, 50),          // Text Position (x, y)
            cv::FONT_HERSHEY_SIMPLEX,   // Font Type
            1.0,                        // Font scale
            cv::Scalar(0, 0, 255),      // Text Color (RED in BGR)
            2                           // Thickness 
        );

        // Visulations Windows 
        cv::imshow("Camera", frame);    // Full annotated frame
        cv::imshow("ROI", roi);         // Region used for decision

        // Exit Condition
        //  - WaitKey(1): allows GUI refresh
        //  - 27 = ESC Key

        if(cv::waitKey(1) == 27)
        {
            break; // Exit mainprocessing loop
        }
        
    }

}