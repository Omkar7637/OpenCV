#!/bin/bash

REPO="/home/omkar/Desktop/OpenCV"
LOCKFILE="/tmp/opencv_git.lock"

cd "$REPO" || exit 1
TIMESTAMP=$(date "+%Y-%m-%d %H:%M:%S")

# ---------- Prevent parallel runs ----------

if [ -f "$LOCKFILE" ]; then
echo "[$TIMESTAMP] Previous git process still running. Skipping."
exit 0
fi

touch "$LOCKFILE"

# ---------- Check git health ----------

git fsck --no-progress > /dev/null 2>&1
if [ $? -ne 0 ]; then
echo "[$TIMESTAMP] Git repo unhealthy. Attempting repair..."
git gc --prune=now > /dev/null 2>&1
git repack -a -d > /dev/null 2>&1
fi

# ---------- Heartbeat commit ----------

echo "Last alive: $TIMESTAMP" > .heartbeat
git add .heartbeat
git commit -m "Heartbeat: $TIMESTAMP" > /dev/null 2>&1

# ---------- Real source changes ----------

CHANGES=$(git status --porcelain | grep -vE "(.o|.so|build/|CMakeFiles/|.log|.cache|.heartbeat)")

if [[ -n "$CHANGES" ]]; then
echo "[$TIMESTAMP] Source changes detected"
git add -A
git commit -m "Auto update: $TIMESTAMP" > /dev/null 2>&1
fi

# ---------- Always sync with GitHub ----------

git pull --rebase origin main > /dev/null 2>&1
git push origin main

echo "[$TIMESTAMP] Repository synced"

# ---------- Cleanup ----------

git gc --auto > /dev/null 2>&1
rm -f "$LOCKFILE"

