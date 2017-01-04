import numpy as np
import cv2
from time import time


distanceThreshold=0
distanceThreshold_4_goal=0
file="/dev/ttyACM0"

def distance(line,point):
    point1=line[0]
    point2=line[1]
    dist=(point2[0]-point1[0])*(point[1]-point1[1])-(point2[1]-point1[1])*(point[0]-point1[0])
    ss=sqrt((point2[1]-point1[1])**2 + (point2[0]-point1[0])**2)
    if ss ! = 0:
        return float(dist)/ss

def motorWrite(d):
    try:
        with open (file,'w') as File:
            File.write(d)
        print 1
    except:
        print 0

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

def botdetect(image):
    height, width, channels = image.shape
    fx=fy=f=bx=by=b=0
    for i in height:
        for j in width:
            if image[i,j]==[]:
                fx+=i
                fy+=j
                f+=1
            if image[i,j]==[]:
                bx+=i
                by+=j
                b+=1
    pointf=[fx/f,fy/f]
    pointb=[bx/b,by/b]
    point=[0.5*(fx/f+bx/b),0.5*(fy/f+by/b)]
    return pointf,pointb,point


cap = cv2.VideoCapture(0)

def botMove(pointS,pointE,ledBlink=0):
    line=[pointS,pointE]
    
    while(True):
        ret, frame = cap.read()

        front,back,center=botdetect(frame)
        cv2.line(frame,tuple(center),tuple(front),(255,0,255),1)
        cv2.line(frame,tuple(center),tuple(end),(0,255,255),1)
        
        #chcking if it is present at the goal
        GoalDistance=sqrt((pointE[1]-center[1])**2 + (pointE[0]-center[0])**2)
        if  GoalDistance < distanceThreshold_4_goal:
            if ledBlink:
                motorWrite('b')
            motorWrite('0')
            return True
        
        DFront=distance(line,front)
        DCenter=distance(line,center)
        DBack=distance(line,back)

        #moving the bot along te line
        if Dback < DCenter < DFront :
            theta=180
        if Dback > DCenter > DFront :
            theta=0
        
        if Dback > DCenter < DFront :
            if abs(DCenter) < distanceThreshold : 
                theta=0
            else:
                theta=
        
        
        move(theta)
        cv2.imshow('bot',frame)
        if cv2.waitKey(1) & 0xFF == 27:
            break


cap.release()
cv2.destroyAllWindows()
