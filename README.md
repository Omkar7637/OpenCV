
# OpenCV – C++ & Python on NVIDIA Jetson Nano (Super 8GB) 🚀

This repository contains my **OpenCV learning, experiments, and real-time camera
applications** using **C++ and Python**, developed and tested on  
**NVIDIA Jetson Nano Super 8GB** running **Ubuntu Linux**.

The focus is on **embedded vision**, **camera pipelines**, and **real-time image
processing**.

---

## 🧠 Platform Details
- **Board**: NVIDIA Jetson Nano **Super 8GB**
- **OS**: Ubuntu (JetPack based)
- **Camera**: CSI Camera (via GStreamer)
- **Language**: C++, Python
- **Library**: OpenCV 4.x

---

## ✨ Features
- Image loading and display
- Color space conversion (BGR → Grayscale)
- Gaussian Blur
- Edge Detection (Canny)
- Thresholding (Binary / OTSU)
- Real-time camera capture using OpenCV
- CSI Camera integration using GStreamer
- C++ and Python OpenCV examples
- Designed for **embedded vision on Jetson Nano**

---

## 🛠️ Requirements
- Ubuntu 20.04 / 22.04 (Jetson)
- OpenCV 4.x
- GCC / G++
- Python 3.x

Check OpenCV version:
```bash
pkg-config --modversion opencv4
````

---

## ⚙️ Build & Run (C++)

### Compile

```bash
g++ filename.cpp -o output `pkg-config --cflags --libs opencv4`
```

### Run

```bash
./output
```

### Example

```bash
g++ 04-Blur_demo.cpp -o blur `pkg-config --cflags --libs opencv4`
./blur
```

---

## 📷 CSI Camera (Jetson Nano)

Example **GStreamer pipeline** used with OpenCV on Jetson Nano:

```cpp
cv::VideoCapture cap(
    "nvarguscamerasrc ! video/x-raw(memory:NVMM), width=1920, height=1080, framerate=60/1 ! "
    "nvvidconv ! video/x-raw, format=BGRx ! videoconvert ! video/x-raw, format=BGR ! appsink",
    cv::CAP_GSTREAMER
);
```

This pipeline is optimized for **Jetson Nano CSI camera access**.

---

## 📂 Recommended Project Structure

```
OpenCV/
├── cpp/                # C++ OpenCV examples
├── python/             # Python OpenCV examples
├── scripts/            # Build / helper scripts
├── .gitignore
└── README.md
```

---

## 🎯 Purpose of This Repository

* Learning OpenCV fundamentals
* Practicing real-time image processing
* Embedded vision development on Jetson Nano
* Reference for robotics, AI, and vision projects
* Portfolio & interview showcase

---

## 👤 Author

**Omkar Ankush Kashid**
Embedded Software Developer

* GitHub: [https://github.com/Omkar7637](https://github.com/Omkar7637)
* Skills: Embedded C, C++, OpenCV, Linux, Jetson, STM32, ESP32

````
