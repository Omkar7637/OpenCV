/*****************************************************************************************
*   File Name   : Clear_Path.cpp
*   Discription : Path finding for IGV using OpenCV and GStreamer
*                This application captures frames from the Jetson Nano CSI
*                camera, processes them using OpenCV, and displays the output.
*
*   Author      : Omkar Ankush Kashid
*   Created On  : 29-12-2025
*   Last Updated: 05-01-2026
*
*   Platform    : NVIDIA Jetson Nano Super 8GB
*   OS          : Ubuntu (JetPack)
*   Language    : C++
* 
*****************************************************************************************/

// ======================== Header Files ==============================
#include <opencv2/opencv.hpp> // Realted to OpenCV 
#include <iostream> // Standered I/O stream Library

int main()
{
    // Local Variable declartions
    // OpenCV realted Variables
    cv::Mat frame, gray, binary, roi;

    // Code

    // ============ CSI Camera Pipeline ============

    // CSI Camera Pipeline Required for the jetson nano
    std::cout << std::endl;
    std::cout << "----- IGV::Pipeline Initialization Start -----" << std::endl;
    cv::VideoCapture cap(
        "nvarguscamerasrc !" // Direct interface to the navidia driver (Most setting Auto)
        
        "video/x-raw(memory:NVMM)," // NVIDIA Multimedia memory | Critical for the High FPS/Low Latency/AI Interface Pipelines
        // "width=640, height=480, frame=60/1 ! "
        "width=1920, height=1080, frame=60/1 ! "
        "nvvidconv !" // Uses jetson hardware | Much faster than videconverter | Converter: memory layout/color formats/resolution
        "video/x-raw, format=BGRx !" // BGR + unused alpha channel required videoconverter works efficently with BGRx 
        " videoconvert !" // CPU-based color format converter | converts BGRx -> BGR | OpenCV expects BGR format
        " video/x-raw, format=BGR !" // Final format passed to the OpenCV | 3-Channel BGR image (CV_8UC3)
        " appsink ", // Sink element that allows application (OpenCV) to read frames | Without appsink, OpenCV cannot access the video stream

        // ====== BackEnd Selection ======
        cv::CAP_GSTREAMER // Force OpenCV to use the GStreamer backend | Mandatory when passing a GStreamer pipeline string  

    );

    std::cout << "----- IGV::Camera Piepline Successfully intialized -----" << std::endl;

    // =============== Camera Access Check ===============
    //
    // - GStrame Pipeline creation | Camera avilabilty 
    // - Successful negotiation of caps (resolution, FPS, Format)

    if(!cap.isOpened()) // If any element in the pipeline fails, this return false
    {
        std::cout << "----- Camera Not Supported!! -----" << std::endl;
        return (EXIT_FAILURE); // Exit Program safely
    }

    std::cout <<  "----- IGV::Camera Working -----" << std::endl;

    std::cout << "===================================================" << std::endl;
    std::cout << "\n----- IGV::Pipeline -----" << std::endl;
    std::cout << "CSI Camera Sensor\n       |       \nnvarguscamerasrc\n       |       \nNVMM (GPU Memory)\n       |       \nnvvidconv (HW Accelearted)\n       |       \n      BGRx\n       |       \nvideoconvertor (CPU)\n       |       \n    appsink\n       |       \nOpenCV cv::Mat\n" << std::endl;
    std::cout << "===================================================" << std::endl;

    // Pipeline Breakdown
    // CSI Camera Sensor -> nvarguscamerasrc -> NVMM (GPU Memory) -> nvvidcon (HW Accelerated) -> BGRx -> videconvert (CPU) -> BGR -> appsink -> OpenCV cv::Mat 

    // Creating Full Screen Winodw to show output
    // std::cout << "----- IGV::Window Creation -----" << std::endl;
    // cv::namedWindow("IGV-Camera", cv::WINDOW_NORMAL);
    // cv::setWindowProperty("IGV-Camera", cv::WND_PROP_FULLSCREEN, cv::WINDOW_FULLSCREEN);
    // std::cout << "----- IGV::Window Created -----" << std::endl;

    // While Loop for Video Each Frame Scanning
    std::cout << "----- IGV::Entering While Loop -----" << std::endl;
    while(true)
    {
        // Take Single frame
        // Camera -> frame
        cap >> frame;

        // Check if frame is empty
        if(frame.empty())
        {
            std::cerr << "IGV::ERROR::Empty Frame received!" << std::endl;
            return(EXIT_FAILURE); 
        }

        // Step 1.Convert color image (BGR Format) into a grayscale
        // frame -> grayscale
        cv::cvtColor(
            frame,                  // Input: Original BGR image from camera
            gray,                   // Output: Single-cahnnel (Grayscale image)
            cv::COLOR_BGR2GRAY      // Conversion type: BGR -> Grayscale
        );

        // Step 2.Apply Gaussian Blur to the Grayscale image
        // Grayscale -> Gaussian Blur
        cv::GaussianBlur(
            gray,                   // Input: grayscale image
            gray,                   // Output: Same image overwritten (in-place)
            cv::Size(5, 5),         // 5x5 Gaussian kernal 
            0                       // Auto calculate Sigma
        );

        // Step 3.Apply thershold to convert a grayscale image a binary image
        // Gaussian blur -> Thersholding
        // This converts | 0(Black) | 255(White)
        cv::threshold(
            gray,                   // Input Grayscale Image 
            binary,                 // Output Binary Image
            0,                      // Thershold value (not used by OTSU)
            255,                    // Maximum Value for THRESH_BINARY
            cv::THRESH_BINARY |     // Binary thersholding 
            cv::THRESH_OTSU         // Automatically calculates best thershold
        );

        // Step 4.Define ROI
        // ROI = Region of Interest 
        // Insted of processing thr entire image, we process only a spacific region
        // Here: The bottom half of the image (where the robot path is expected)

        int h = binary.rows; // Total height of the binary image (In pixels)
        int w = binary.cols; // Total width of the binary image (In Pixels)

        // Create ROI using cv::react
        // cv::Rect(x, y, width, height)
        roi = binary(cv::Rect(
            0,                      // Start from left edge
            h/2,                    // Start from middle vertically
            w,                      // full width
            h/2                     // bottom half of the image
        )); 

        // Show results
        // cv::imshow("Origianl", frame);
        // cv::imshow("Binary", binary);
        cv::imshow("ROI (Path Area)", roi);

        if(cv::waitKey(1) == 27) // Waits 1 millisecond | Allows window refresh | Return ASCII value of pressed key
            break;
    }

    std::cout << "----- IGV::Exiting While Loop -----" << std::endl;

    std::cout << "----- Exiting Sucessfully -----" << std::endl;
    return(EXIT_SUCCESS);
}