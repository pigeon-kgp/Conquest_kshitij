import cv2
import numpy as np
from math import sqrt
from copy import copy


def AssignColor(event,x,y,flags,param):
    if event==cv2.EVENT_LBUTTONDOWN:
        list.append(img[x,y])




def harris(img):
    #img=copy(image)
    gray = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
    dst = cv2.cornerHarris(gray,2,3,0.04)
    dst = cv2.dilate(dst,None)
    height, width, channels = img.shape
    img[dst>0.01*dst.max()]=[0,0,255]
    return img

def blob__Detec(image):
    img=copy(image)
    height, width, channels = img.shape
    new_img=np.ones((height,width,channels), np.uint8)
    HSV = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
    Yellow={'min':(20,100,100),'max':(30, 255, 255)}
    Blue={'min':(50,100,100),'max':(100,255,255)}
    Brown={'min':(0,100,0),'max':(20,255,255)}

    mask_b=cv2.inRange(HSV,Blue['min'],Blue['max'])
    mask_br=cv2.inRange(HSV,Brown['min'],Brown['max'])
    mask_y=cv2.inRange(HSV,Yellow['min'],Yellow['max'])

    blue=cv2.bitwise_and(img,img,mask=mask_b)
    yellow=cv2.bitwise_and(img,img,mask=mask_y)
    brown=cv2.bitwise_and(img,img,mask=mask_br)

    new_img=cv2.add(blue,brown)
    new_img=cv2.add(new_img,yellow)

    return new_img



def blob__Detec__location(image):
    min_area=int(raw_input("Min Area of blobs: "))
    max_area=int(raw_input("Max Area of blobs: "))
    t1=200
    t2=0
    img=copy(image)
    #new_img=copy(newimg)
    height, width, channels = img.shape
    new_img=np.ones((height,width,channels), np.uint8)
    HSV = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
    Yellow={'min':(20,100,100),'max':(30, 255, 255)}
    Blue={'min':(50,100,100),'max':(100,255,255)}
    Brown={'min':(0,100,0),'max':(20,255,255)}

    yellow=cv2.inRange(HSV,Yellow['min'],Yellow['max'])
    blue=cv2.inRange(HSV,Blue['min'],Blue['max'])
    brown=cv2.inRange(HSV,Brown['min'],Brown['max'])

    params = cv2.SimpleBlobDetector_Params()
    params.filterByArea = True
    params.minArea=min_area
    params.maxArea=max_area+t1

    detector=cv2.SimpleBlobDetector(params)
    woods=detector.detect(255-yellow)
    for i in woods:
                x=i.pt[0]; y=i.pt[1]
                woodList.append([x,y])
    for i in woods:
        new_img[int(i.pt[1]),int(i.pt[0])]=[0,255,0]
        new_img[int(i.pt[1])+1,int(i.pt[0])+1]=[0,255,0]
        new_img[int(i.pt[1])+1,int(i.pt[0])]=[0,255,0]
        new_img[int(i.pt[1]),int(i.pt[0]+1)]=[0,255,0]

    params.minArea=max_area+t2
    params.maxArea=250

    detector=cv2.SimpleBlobDetector(params)
    food=detector.detect(255-yellow)
    for i in food:
            x=i.pt[0]; y=i.pt[1]
            foodList.append([x,y])
    for i in food:
        new_img[int(i.pt[1]),int(i.pt[0])]=[255,255,0]
        new_img[int(i.pt[1])+1,int(i.pt[0])+1]=[255,255,0]
        new_img[int(i.pt[1])+1,int(i.pt[0])]=[255,255,0]
        new_img[int(i.pt[1]),int(i.pt[0]+1)]=[255,255,0]


    params=cv2.SimpleBlobDetector()
    river=params.detect(255-blue)
    img_river=np.zeros((height,width,channels), np.uint8)

    s=int(raw_input("enter area\n"))
    s=int(sqrt(s))
    for i in river:
        x=int(i.pt[0])
        y=int(i.pt[1])
        riverList.append([x,y])
        for i in xrange(-s,s):
            for j in xrange(-s,s):
                try:
                    img_river[y+j,x+i]=[255,255,255]
                except:
                    pass
    img_river=harris(img_river)


    #params=cv2.SimpleBlobDetector()
    townCenter=params.detect(255-brown)
    for i in townCenter:
                x=i.pt[0]; y=i.pt[1]
                townList.append([x,y])
    for i in townCenter:
        new_img[int(i.pt[1]),int(i.pt[0])]=[50,100,200]
        new_img[int(i.pt[1])+1,int(i.pt[0])+1]=[50,100,200]
        new_img[int(i.pt[1])+1,int(i.pt[0])]=[50,100,200]
        new_img[int(i.pt[1]),int(i.pt[0]+1)]=[50,100,200]

    new_img=cv2.add(new_img,img_river)
    return new_img




foodList=[]; woodList=[]; townList=[]; riverList=[]
img=cv2.imread("2.png",cv2.IMREAD_COLOR)


blob=blob__Detec(img)
centroid=blob__Detec__location(blob)

cv2.imshow('centroid',centroid)
cv2.imshow('image',blob)
print "\nFood: "+str(foodList)
print "\nWood: "+str(woodList)
print "\nTown: "+str(townList)
print "\nRiver: "+str(riverList)

cv2.waitKey(0)
