#!/bin/bash

# ==============================
# OpenCV Build Script
# Usage: ./build.sh <filename_without_extension>
# Example: ./build.sh 04-Blur_demo
# ==============================

clear

if [ $# -ne 1 ]; then
    echo "Usage: ./build.sh <filename_without_extension>"
    exit 1
fi

SRC="$1.cpp"
OUT="$1"

# Compile
sudo systemctl restart nvargus-daemon
g++ "$SRC" -o "$OUT" `pkg-config --cflags --libs opencv4`

# Check compile status
if [ $? -ne 0 ]; then
    echo "❌ Build failed"
    exit 1
fi

echo "=================================== Build successful ==================================="

# Run
./"$OUT"

rm $OUT

echo "======================================== Done ========================================"
