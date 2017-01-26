import cv2
import numpy as np
fle=open("brown.txt",'w')
def getcolor(event,x,y,flags,param):
    if event == cv2.EVENT_LBUTTONDBLCLK:
        HSV = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
        color=HSV[y,x]
        print color

        fle.write(str(color))

# Create a black image, a window and bind the function to window
frame = np.zeros((512,512,3), np.uint8)
cv2.namedWindow('image')
cv2.setMouseCallback('image',getcolor)
cap = cv2.VideoCapture("/home/ss/Videos/Webcam/abc.webm")
try:
    while(True):
        # Capture frame-by-frame
        ret, frame = cap.read()

        # Our operations on the frame come here
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

        cv2.imshow('image',frame)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
except:
    cv2.destroyAllWindows()
    fle.close()
