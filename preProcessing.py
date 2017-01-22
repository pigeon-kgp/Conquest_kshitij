import cv2
import numpy as np
from math import sqrt
from math import pow
from copy import copy
import os
foodList=[]; woodList=[]; townList=[]; riverList=[]; points=[]
#for 1.png, set it to 1e-12, for 2.png set it to -14
distThresh=0

def plotLine(x1,y1,x2,y2):
    try:
        m=float(y2-y1)/(x2-x1)
        c=y2-m*x2
        x=x1; y=y1; count=0.01
        if (x2<x1): count=-0.01
        olddist=Dist(int(x),int(y),int(x2),int(y2))
        while (True):
            y=(m*x+c)
            if (Dist(int(x),int(y),int(x2),int(y2))>olddist+distThresh):
                return
            else:
                olddist=Dist(int(x),int(y),int(x2),int(y2))
            centroid[int(y),int(x)]=[255,255,0]
            x+=count
    except:
        x=x1; y=y1; count=1
        if (y2<y1): count=-1
        while (True):
            if Dist(x,y,x2,y2)<8:
                return
            centroid[int(y),int(x)]=[255,255,0]
            y+=count

def Disth(item):
        dist=sqrt(((item[0]-townList[0][0])**2)+((item[1]-townList[0][1])**2))
        if item[2]=="food":
            return dist*0.75
        else:
            return dist
def Distn(item):
        dist=sqrt(((globx2-item[0])**2)+((globy2-item[1])**2))
        return dist
def Dist(x1,y1,x2,y2):
        return (sqrt((x1-x2)**2+(y1-y2)**2))

def clearance(xorg,yorg,xfin,yfin):
        try:
            m=float(yfin-yorg)/(xfin-xorg)
            c=yfin-m*xfin
            x=xorg; y=yorg; count=0.01
            if (xfin<xorg): count=-0.01
            olddist=Dist(int(x),int(y),int(xfin),int(yfin))
            while (True):
                y=(m*x+c)
                if (centroid[int(y),int(x)][0]==255 and centroid[int(y),int(x)][1]==255 and centroid[int(y),int(x)][2]==255):
                    return ([int(x),int(y)])
                if (Dist(int(x),int(y),int(xfin),int(yfin))>olddist+distThresh):
                    print Dist(int(x),int(y),int(xfin),int(yfin))-olddist
                    return 1
                else:
                    olddist=Dist(int(x),int(y),int(xfin),int(yfin))
                #centroid[int(y),int(x)]=[255,255,0]
                x+=count
        except:
            x=xorg; y=yorg; count=1
            if (yfin<yorg): count=-1
            while (True):
                    if (centroid[int(y),int(x)][0]==255 and centroid[int(y),int(x)][1]==255 and centroid[int(y),int(x)][2]==255):
                        return ([int(x),int(y)])
                    if Dist(x,y,xfin,yfin)<8:
                        return 1
                    #centroid[int(y),int(x)]=[255,255,0]
                    y+=count

def pathPlanning(town):
    global foodList; global woodList
    netList=foodList+woodList
    netList.sort(key=Disth)
    print "\nNet: "+str(netList)
    print "eurhfuehfiehfkefkejfiej"+str(len(netList))
    print "otherwise: "+str(len(woodList))
    xorg=town[0]; yorg=town[1]; flag=0
    for i in netList:
        #print i
        flag=0
        if (int(i[0])>=480): continue
        pointstemp=[]; riverHere=copy(riverList)
        xfin=i[0]; yfin=i[1]
        globx2=xfin; globy2=yfin
        riverHere.sort(key=Distn)
        while(True):
            if flag==0:
                place=clearance(xorg,yorg,xfin,yfin)
            else:
                place=clearance(pointstemp[len(pointstemp)-1][0],pointstemp[len(pointstemp)-1][1],xfin,yfin)
            if (type(place) is int):
                flag=0
            else:
                #globx=place[0]; globy=place[1]
                for j in riverHere:
                    if (len(pointstemp)>=1):
                        check=clearance(pointstemp[len(pointstemp)-1][0],pointstemp[len(pointstemp)-1][1],j[0],j[1])
                    else:
                        check=clearance(xorg,yorg,j[0],j[1])
                    if (type(check) is int):
                        #globx=j[0]; globy=j[1]
                        print "Yo!"
                        pointstemp.append([j[0],j[1]])
                        riverHere.remove(j)
                        break
                flag=1
            if flag==0: break
        points.extend([[xorg,yorg]]+pointstemp+[[i[0],i[1]],1]+pointstemp[::-1]+[[xorg,yorg]]+[1])
        #points.extend([[xorg,yorg]]+pointstemp+[[i[0],i[1]],1]+pointstemp[::-1]+[[xorg,yorg]]+[1])



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

    #cv2.imwrite("toread2.jpg",yellow)

    """
    os.system("./shapelen")
    myfile=open("rivers.txt",'r')
    while(1):
        line=myfile.readline()
        if not line: break
        riverList.append([int(line.split(' ')[0]),int(line.split(' ')[1])])
    """

    params = cv2.SimpleBlobDetector_Params()
    params.filterByArea = True
    params.minArea=min_area
    params.maxArea=max_area+t1

    detector=cv2.SimpleBlobDetector(params)
    woods=detector.detect(255-yellow)
    for i in woods:
                x=i.pt[0]; y=i.pt[1]
                woodList.append([x,y,"wood"])
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
            foodList.append([x,y,"food"])
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





img=cv2.imread("1.png",cv2.IMREAD_COLOR)
globx=globx2=0; globy=globy2=0

blob=blob__Detec(img)
centroid=blob__Detec__location(blob)

HSV = cv2.cvtColor(centroid, cv2.COLOR_BGR2HSV)
Red={'min':(0,100,100),'max':(20,255,255)}
red=cv2.inRange(HSV,Red['min'],Red['max'])
#params = cv2.SimpleBlobDetector()
#rivers=params.detect(red)
#for i in rivers:
#                x=i.pt[0]; y=i.pt[1]
#                riverList.append([x,y])

cv2.imwrite("toread.jpg",red)
os.system("./shapelen")
myfile=open("rivers.txt",'r')
while(1):
    line=myfile.readline()
    if not line: break
    riverList.append([int(line.split(' ')[0]),int(line.split(' ')[1])])
myfile.close()
cv2.imshow('reds',red)
cv2.imshow('image',blob)
print "\nFood: "+str(foodList)
print "\nWood: "+str(woodList)
print "\nTown: "+str(townList)
print "\nRiver: "+str(riverList)
pathPlanning(townList[0])
while(True):
    try:
        points.remove([])
    except:
        break
print "\nPath: "+str(points)
i=0; j=1
while(1):
    if (type(points[j]) is int): j+=1
    if (j>=len(points)):
        break
    plotLine(points[i][0],points[i][1],points[j][0],points[j][1])
    i=j;j+=1
cv2.imshow('centroid',centroid)
cv2.waitKey(0)
