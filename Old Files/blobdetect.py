import numpy as np
import cv2
# Yellow colour with square shape represents food
# Yellow colour with triangle represents wood
# Blue colour square represents rivers
# Brown colour represents Town Center
def blobdetect(img):
    hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)

    yellowMin = (20,100,100)
    yellowMax = (30, 255, 255)

    blueMin=(50,100,100)
    blueMax=(100,255,255)

    brownMin=(0,100,0)
    brownMax=(20,255,255)
    yellow=cv2.inRange(hsv,yellowMin, yellowMax)
    blue=cv2.inRange(hsv,blueMin,blueMax)
    brown=cv2.inRange(hsv,brownMin,brownMax)

    params = cv2.SimpleBlobDetector_Params()
    params.filterByArea = True
    params.minArea=150
    detector=cv2.SimpleBlobDetector(params)
    keypoints=detector.detect(255-yellow)
    food=[]
    for i in keypoints:
        x=i.pt[0]; y=i.pt[1]
        food.append([x,y])

    params.maxArea=250
    detector=cv2.SimpleBlobDetector(params)
    keypoints=detector.detect(255-yellow)

    wood=[]
    for i in keypoints:
        x=i.pt[0]; y=i.pt[1]
        wood.append([x,y])

    params=cv2.SimpleBlobDetector()
    keypoints=params.detect(255-blue)
    rivers=[]
    for i in keypoints:
        x=i.pt[0]; y=i.pt[1]
        rivers.append([x,y])

    keypoints=params.detect(255-brown)
    centre=[]
    for i in keypoints:
        x=i.pt[0]; y=i.pt[1]
        centre.append([x,y])

    return [food, wood, rivers, centre]


img = cv2.imread("2.png", cv2.IMREAD_COLOR)
places=blobdetect(img)

print "Food: "+str(places[0])+"\n"
print "Wood:" +str(places[1])+"\n"
print "Rivers:" +str(places[2])+"\n"
print "Centre:" +str(places[3])+"\n"
cv2.imshow("Image",img)
cv2.waitKey(0)