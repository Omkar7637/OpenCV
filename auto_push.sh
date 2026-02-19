#!/bin/bash

# ================================
# Auto Git Commit & Push Script
# Repo: OpenCV
# Platform: Jetson Nano Super 8GB
# Author: Omkar Ankush Kashid
# ================================

# Move to repo directory (important for cron)
cd /home/omkar/Desktop/OpenCV || exit 1

# Get current timestamp
TIMESTAMP=$(date "+%Y-%m-%d %H:%M:%S")

# Check if there are any changes
if [[ -n $(git status --porcelain) ]]; then
    git add .
    git commit -m "Auto update: $TIMESTAMP"
    git push origin main
else
    echo "[$TIMESTAMP] No changes to push"
fi

