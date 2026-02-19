import cv2

print(cv2.__version__)

img = cv2.imread("test_img.jpg")   # ✅ correct file

if img is None:
    print("Image not loaded")
    exit()

cv2.imshow("Output", img)
cv2.waitKey(0)
cv2.destroyAllWindows()

