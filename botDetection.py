import numpy as np
import cv2
from time import time
from copy import copy

distanceThreshold=0
distanceThreshold_4_goal=0
turningTreshold=0

file="/dev/ttyACM0"

ORANGE={'min':(8,119,223),'max':(11,118,229)}
PINK={'min':(158,126,196),'max':(162,106,211)}

orange={'min':(8,119,223),'max':(11,118,229)}
pink={'min':(158,126,196),'max':(162,106,211)}

WindowName='botImage'


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

cv2.createTrackbar('Orangemin',WindowName,0,50,change)
cv2.createTrackbar('Orangemax',WindowName,0,50,change)
cv2.createTrackbar('Pinkmin',WindowName,0,50,change)
cv2.createTrackbar('Pinkmax',WindowName,0,50,change)

def distance(line,point):
    point1=line[0]
    point2=line[1]
    dist=(point2[0]-point1[0])*(point[1]-point1[1])-(point2[1]-point1[1])*(point[0]-point1[0])
    ss=sqrt((point2[1]-point1[1])**2 + (point2[0]-point1[0])**2)
    if not s==0:
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
    new_img=np.ones((height,width,channels), np.uint8)
    HSV = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
    
    orange_part=cv2.inRange(HSV,orange['min'],orange['max'])
    pink_part=cv2.inRange(HSV,pink['min'],pink['max'])
    
    new_img=cv2.add(pink,orange)

    height, width, channels = new_img.shape
    fx = fy = f = bx = by = b = 0.1
    new_img=cv2.cvtColor(image,cv2.COLOR_BGR2HSV)

    for y in range(height):
        for x in range(width):
            if new_img[y,x][0] < pink['max'][0] and new_img[y,x][0] > pink['min'][0]  :
                fx+=y
                fy+=x
                f+=1
            if new_img[y,x][0] < orange['max'][0] and new_img[y,x][0] > orange['min'][0]:
                bx+=y
                by+=x
                b+=1
    pointf = [fx/f,fy/f]
    pointb = [bx/b,by/b]
    point = [0.5*(fx/f+bx/b),0.5*(fy/f+by/b)]
    return pointf,pointb,point



def PathTracer(path):
    i = 0
    while(i+1<len(path)):
        point1 = path[i]
        point2 = path[i+1]
        botMove(point1,point2)
        i+= 1
    

cap  =  cv2.VideoCapture('conq2.webm')

def botMove(pointS,pointE):
    line = [pointS,pointE]
    ledBlink = pointE[2]
    while(True):
        ret, frame  =  cap.read()
        motorWrite('S')

        cv2.imshow(WindowName,frame)
        if cv2.waitKey(1) & 0xFF == 27:
            break

        front,back,center=botdetect(frame)

        cv2.line(frame,tuple(center),tuple(front),(255,0,255),1)
        cv2.line(frame,tuple(center),tuple(end),(0,255,255),1)
        
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

        #moving the bot along te line
        if DBack < DCenter < DFront :
            print('back')
            motorWrite('s')
        if DBack > DCenter > DFront :
            print('forward')
            motorWrite('w')
        
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
        
        
        #move(theta)

        cv2.imshow(WindowName,frame)
        if cv2.waitKey(1) & 0xFF == 27:
            break

botMove([9,0],[8,9,1])
cv2.namedWindow(WindowName)
cap.release()
cv2.destroyAllWindows()
