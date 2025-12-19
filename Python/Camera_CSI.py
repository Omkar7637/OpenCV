import cv2

pipeline = (
    "nvarguscamerasrc ! "
    "nvvidconv ! "
    "videoconvert ! "
    "appsink"
)

cap = cv2.VideoCapture(pipeline, cv2.CAP_GSTREAMER)
print("Opened:", cap.isOpened())

while cap.isOpened():
    ret, frame = cap.read()
    if not ret:
        break
    cv2.imshow("CSI Camera", frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()

