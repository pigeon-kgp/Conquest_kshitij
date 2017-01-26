import numpy as np
import cv2
from time import time
from copy import copy
from math import sqrt

distanceThreshold=0
distanceThreshold_4_goal=0
turningTreshold=0

file="/dev/ttyACM0"

ORANGE={'min':(5,50,223),'max':(15,118,250)}
PINK={'min':(150,70,140),'max':(165,150,221)}

orange={'min':(5,50,223),'max':(15,118,250)}
pink={'min':(150,50,120),'max':(165,140,221)}

WindowName='botImage'

cv2.namedWindow("timon")
def getcolor(event,x,y,flags,param):
        if event == cv2.EVENT_LBUTTONDBLCLK:
            HSV = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
            color=HSV[y,x]
            print color
            cv2.waitKey(0)

cv2.setMouseCallback('timon',getcolor)

def change():
 global ORANGE,orange,PINK,pink
 TrackbarPos = cv2.getTrackbarPos('Orangemin', WindowName)
 orange['min'][0]=ORANGE['min'][0]+TrackbarPos

 TrackbarPos = cv2.getTrackbarPos('Orangemax', WindowName)
 orange['max'][0]=ORANGE['max'][0]+TrackbarPos

 TrackbarPos = cv2.getTrackbarPos('Pinkmin', WindowName)
 pink['min'][0]=PINK['min'][0]+TrackbarPos

 TrackbarPos = cv2.getTrackbarPos('Pinkmax', WindowName)
 pink['max'][0]=PINK['max'][0]+TrackbarPos
'''
cv2.createTrackbar('Orangemin',WindowName,0,50,change)
cv2.createTrackbar('Orangemax',WindowName,0,50,change)
cv2.createTrackbar('Pinkmin',WindowName,0,50,change)
cv2.createTrackbar('Pinkmax',WindowName,0,50,change)
'''
def distance(line,point):
    point1=line[0]
    point2=line[1]
    dist=(point2[0]-point1[0])*(point[1]-point1[1])-(point2[1]-point1[1])*(point[0]-point1[0])
    ss=sqrt((point2[1]-point1[1])**2 + (point2[0]-point1[0])**2)
    if not ss==0:
        return float(dist)/ss
    else:
	return 0

def motorWrite(d):
    try:
        with open (file,'w') as File:
            File.write(d)
        print (d)
    except:
        print (0)
'''
def move(theta):
    if theta == 0:
        motorWrite('w')
        return
    elif theta == 180 or theta == -180 :
        motorWrite('s')
        return
    elif theta < 0 :
        d='a'
    else :
        d='d'

    th=abs(theta)

    startTime=time()
    while(True):
        currentTime=time()
        if currentTime-startTime >= th:
            break
        motorWrite(d)
'''
def botdetect(image):
    global ORANGE,orange,PINK,pink
    img=copy(image)


    #new_img=copy(newimg)
    height, width, channels = img.shape
    #new_img=np.ones((height,width,channels), np.uint8)
    HSV = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)

    orange_part=cv2.inRange(HSV,orange['min'],orange['max'])
    global pink_part
    pink_part=cv2.inRange(HSV,pink['min'],pink['max'])

    #new_img=cv2.add(pink_part,orange_part)
    #cv2.imshow("haha",new_img)
    #cv2.waitKey(1)
    #height, width, channels = new_img.shape
    fx = fy = f = bx = by = b = 0.1
    #new_img=cv2.cvtColor(image,cv2.COLOR_BGR2HSV)
    count=0
    print "new_img"
    params = cv2.SimpleBlobDetector_Params()
    params.filterByArea = True
    params.minArea=1
    params.maxArea=4
    detector=cv2.SimpleBlobDetector(params)
    pinky=detector.detect(255-pink_part)
    orangy=detector.detect(255-orange_part)
    cv2.imshow("pinky",pink_part+orange_part)
    cv2.waitKey(1)
    print pinky,orangy
    kpx=kpy=korx=kory=0
    for kpink in pinky:
        kpx+=kpink.pt[0]
        kpy+=kpink.pt[1]
    try:
        kpx/=len(pinky)
        kpy/=len(pinky)
    except:
        
        return -1,-1,-1

    for korange in orangy:
        korx+=korange.pt[0]
        kory+=korange.pt[1]
    try:
        korx/=len(orangy)
        kory/=len(orangy)
    except:
        return -1,-1,-1
    try:

        pointf=[int(kpx),int(kpy)]
        pointb=[int(korx),int(kory)]
        point=[int((kpx+korx)/2),int((kpy+kory)/2)]
        print("Peace!")
        cv2.waitKey(1)

    except:
        return -1, -1, -1

    '''
    for y in range(height):
        for x in range(width):
            #if pink_part[y,x] <= pink['max'][0] and pink_part[y,x][0] > pink['min'][0]  :
            if pink_part[y,x] <= 3  :
                count+=1
                fx+=y
                fy+=x
                f+=1
            #if orange_part[y,x] < orange['max'][0] and orange_part[y,x] > orange['min'][0]:
            if orange_part[y,x] <= 3  :
                count+=1
                bx+=y
                by+=x
                b+=1
    if count==0:

        return -1,-1,-1
    pointf = [int(fx/f),int(fy/f)]
    pointb = [int(bx/b),int(by/b)]
    point = [int(0.5*(fx/f+bx/b)),int(0.5*(fy/f+by/b))]
    '''
    print "returned"
    return pointf,pointb,point



def PathTracer(path):
    i = 0
    while(i+1<len(path)):
        point1 = path[i]
        point2 = path[i+1]
        botMove(point1,point2)
        i+= 1


cap  =  cv2.VideoCapture('./abc2.mp4')

def botMove(pointS,pointE):
    line = [pointS,pointE]
    ledBlink = pointE[2]
    while(True):
        global frame
        ret, frame  =  cap.read()
        motorWrite('S')

        cv2.imshow("timon",frame)
        print "1"
        if cv2.waitKey(1) & 0xFF == 27:
            break

        front,back,center=botdetect(frame)
        if front==-1: continue
        print "2"
        cv2.line(frame,tuple(center),tuple(front),(255,0,255),1)
        cv2.line(frame,tuple(center),tuple(back),(0,255,255),1)
        print "3"
        #chcking if bot is present at the goal
        GoalDistance=sqrt((pointE[1]-center[1])**2 + (pointE[0]-center[0])**2)
        if  GoalDistance < distanceThreshold_4_goal:
            if ledBlink:
                print('bulb')
                motorWrite('b')
            #motorWrite('0')
            return True

        DFront=distance(line,front)
        DCenter=distance(line,center)
        DBack=distance(line,back)
        print "4"
        #moving the bot along te line
        if DBack < DCenter < DFront :
            print('back')
            motorWrite('s')
        if DBack > DCenter > DFront :
            print('forward')
            motorWrite('w')
        print "5"
        if DBack > DCenter < DFront :
            if abs(DCenter) < distanceThreshold :
            	print('forward')
                motorWrite('w')
            elif DFront-DBack >= turningThreshold:
            	print('left')
            	motorWrite('a')
            elif DBack-DFront >= turningThreshold:
                print('right')
            	motorWrite('d')
        print "6"

        #move(theta)

        cv2.imshow(WindowName,frame)
        if cv2.waitKey(1) & 0xFF == 27:
            break

botMove([9,0],[8,9,1])
cv2.namedWindow(WindowName)
cap.release()
cv2.destroyAllWindows()
