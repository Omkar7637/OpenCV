#!/bin/bash

# ================================

# SAFE Auto Git Commit & Push Script

# Self-healing + Lock protected

# ================================

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

```
git gc --prune=now > /dev/null 2>&1
git repack -a -d > /dev/null 2>&1
```

fi

# ---------- Heartbeat (keep repo alive) ----------

echo "Last alive: $TIMESTAMP" > .heartbeat

git add .heartbeat
git commit -m "Heartbeat: $TIMESTAMP" > /dev/null 2>&1



# ---------- Detect real file changes ----------

CHANGES=$(git status --porcelain | grep -vE "(.o|.so|build/|CMakeFiles/|.log|.cache)")

if [[ -n "$CHANGES" ]]; then
echo "[$TIMESTAMP] Changes detected"

```
git add -A
git commit -m "Auto update: $TIMESTAMP" > /dev/null 2>&1

# Pull before push (prevents remote conflicts)
git pull --rebase origin main > /dev/null 2>&1

git push origin main

echo "[$TIMESTAMP] Push completed"
```

else
echo "[$TIMESTAMP] No source changes"
fi

# ---------- Periodic cleanup ----------

git gc --auto > /dev/null 2>&1

rm -f "$LOCKFILE"

