import cv2
import numpy as np
from math import sqrt
from math import pow
from copy import copy
import os
import colorsys

globx=globx2=0; globy=globy2=0
#for 1.png, set it to 1e-12, for 2.png set it to -14
distThresh=0
def hsv2rgb(h,s,v):
    return tuple(int(i * 255) for i in colorsys.hsv_to_rgb(h,s,v))

def plotLine(x1,y1,x2,y2):
    x=x1
    count=0.005
    try:
        m=float(y2-y1)/(x2-x1)
        c=y2-m*x2
        if x2<x1: count*=-1
        while (True):
            x+=count
            y=m*x+c
            if (count>0 and x>=x2) or (count<0 and x<=x2):
                return
            centroid[int(y),int(x)]=[255,255,0]
    except:
        y=y1
        if (y2<y1): count*=-1
        while (True):
            y+=count
            if (count>0 and y>=y2) or (count<0 and y<=y2):
                return
            centroid[int(y),int(x)]=[255,255,0]

def Disth(item):
        dist=sqrt(((item[0]-town[0])**2)+((item[1]-town[1])**2))
        if item[2]=="food":
            return dist*0.75
        else:
            return dist
def Distn(item):
        print str(globx2)+"sdgfidsfdskjjdkjdsfkjdsbfjdfdsfjdsfdsjdsfdsfjdsjhjdsbvjfdsvbfcjfdsvjhcvdsbjcdshvcjdsvcj"
        global globx2,globy2
        dist=sqrt(((globx2-item[0])**2)+((globy2-item[1])**2))
        return dist
def Dist(x1,y1,x2,y2):
        return (sqrt((x1-x2)**2+(y1-y2)**2))

def clearance(x1,y1,x2,y2):
    x=x1
    count=0.005
    try:
        m=float(y2-y1)/(x2-x1)
        c=y2-m*x2
        if x2<x1: count*=-1
        while (True):
            x+=count
            y=m*x+c
            if (count>0 and x>=x2) or (count<0 and x<=x2):
                return 1
            if (centroid[int(y),int(x)][0]==255 and centroid[int(y),int(x)][1]==255 and centroid[int(y),int(x)][2]==255):
                return 0
    except:
        y=y1
        if (y2<y1): count*=-1
        while (True):
            y+=count
            if (count>0 and y>=y2) or (count<0 and y<=y2):
                return 1
            if (centroid[int(y),int(x)][0]==255 and centroid[int(y),int(x)][1]==255 and centroid[int(y),int(x)][2]==255):
                return 0


def pathPlanning():
    global foodList, woodList, townList, riverList, town
    global globx2,globy2
    netList=foodList+woodList
    netList.sort(key=Disth)
    print "\nNet: "+str(netList)
    print "eurhfuehfiehfkefkejfiej"+str(len(netList))
    print "otherwise: "+str(len(woodList))
    xorg=int(town[0]); yorg=int(town[1]); flag=0
    counted=1

    #riverList.remove([xorg,yorg])
    print str(xorg)+","+str(yorg)
    for i in netList:
        print counted; counted+=1
        print i
        flag=0
        #if (int(i[0])>=480): continue
        pointstemp=[]; riverHere=copy(riverList)
        xfin=i[0]; yfin=i[1]
        globx2=xfin; globy2=yfin
        riverHere.sort(key=Distn)
        while(True):
            print "\n"+str(riverHere)
            if flag==0:
                place=clearance(town[0],town[1],xfin,yfin)
            else:
                place=clearance(pointstemp[len(pointstemp)-1][0],pointstemp[len(pointstemp)-1][1],xfin,yfin)
            if ( (place) == 1):
                flag=0
            else:
                #globx=place[0]; globy=place[1]
                flaghere=1
                for j in riverHere:
                    if (len(pointstemp)>0):
                        if pointstemp[len(pointstemp)-1][0]==j[0] and pointstemp[len(pointstemp)-1][1]==j[1] :
                            continue
                        check=clearance(pointstemp[len(pointstemp)-1][0],pointstemp[len(pointstemp)-1][1],j[0],j[1])
                    else:
                        check=clearance(town[0],town[1],j[0],j[1])
                    if ( (check) == 1):
                        flaghere=0
                        #globx=j[0]; globy=j[1]
                        #print "Yo!"
                        pointstemp.append([j[0],j[1]])
                        riverHere.remove(j)
                        flag=1
                        break
                if (flaghere==1): break

            if flag==0: break
        p=1; q=0

        while(True):
            try:
                print str(pointstemp[q])+"    "+ str(pointstemp[q+1])
                x=clearance(town[0],town[1],pointstemp[q][0],pointstemp[q][1])
                xnext=clearance(town[0],town[1],pointstemp[q+1][0],pointstemp[q+1][1])
                print str(x)+"    "+str(xnext)
                if (x == 1) and (xnext == 1):
                    pointstemp.remove(pointstemp[q])
                    q-=1
            except:
                break
            q+=1
        points.extend([[xorg,yorg]]+pointstemp+[[i[0],i[1]],1,[i[0],i[1]]]+pointstemp[::-1]+[[xorg,yorg]]+[1])
        points.extend([[xorg,yorg]]+pointstemp+[[i[0],i[1]],1,[i[0],i[1]]]+pointstemp[::-1]+[[xorg,yorg]]+[1])


def draw():
    global riverListhere, area, centroid, riverList
    s=int(sqrt(area)/2)
    print ("Hello")
    for i in riverListhere:
        print "Hello again!"
        x=int(i[0])
        y=int(i[1])
        try:
            centroid[y+(s+10),x+(s+10)]=[0,0,255]
            riverList.append([x+s+10,y+s+10])
        except: pass
        try: centroid[y+(s+10),x-(s+10)]=[0,0,255]; riverList.append([x-(s+10),y+(s+10)])
        except: pass
        try: centroid[y-(s+10),x+(s+10)]=[0,0,255]; riverList.append([x+(s+10),y-(s+10)])
        except: pass
        try: centroid[y-(s+10),x-(s+10)]=[0,0,255]; riverList.append([x-(s+10),y-(s+10)])
        except: pass
        try: centroid[y+(s-10),x+(s-10)]=[0,0,255]; riverList.append([x+(s-10),y+(s-10)])
        except: pass
        try: centroid[y+(s-10),x-(s-10)]=[0,0,255]; riverList.append([x-(s-10),y+(s-10)])
        except: pass
        try: centroid[y-(s-10),x+(s-10)]=[0,0,255]; riverList.append([x+(s-10),y-(s-10)])
        except: pass
        try: centroid[y-(s-10),x-(s-10)]=[0,0,255]; riverList.append([x-(s-10),y-(s-10)])
        except: pass
        for i in xrange(-s,s):
            for j in xrange(-s,s):
                try:
                    centroid[y+j,x+i]=[255,255,255]
                except:
                    pass

def plot():
    global woodList, foodList, town
    for i in woodList:
        centroid[int(i[1]),int(i[0])]=[0,255,0]
        centroid[int(i[1])+1,int(i[0])+1]=[0,255,0]
        centroid[int(i[1])+1,int(i[0])]=[0,255,0]
        centroid[int(i[1]),int(i[0]+1)]=[0,255,0]
    for i in foodList:
        centroid[int(i[1]),int(i[0])]=[255,255,0]
        centroid[int(i[1])+1,int(i[0])+1]=[255,255,0]
        centroid[int(i[1])+1,int(i[0])]=[255,255,0]
        centroid[int(i[1]),int(i[0]+1)]=[255,255,0]
    centroid[int(town[1]),int(town[0])]=[255,255,0]
    centroid[int(town[1])+1,int(town[0])+1]=[255,255,0]
    centroid[int(town[1])+1,int(town[0])]=[255,255,0]
    centroid[int(town[1]),int(town[0]+1)]=[255,255,0]
riverListhere=[[458, 366], [458, 341], [434, 341], [411, 341], [458, 315], [508, 287], [485, 287], [458, 287], [158, 177], [158, 152], [207, 129], [184, 129], [157, 129], [207, 104], [207, 80], [206, 53]]
riverList=[]
woodList=[[236.0, 468.2773132324219, 'wood'], [204.0, 468.2773132324219, 'wood'], [171.0, 468.2773132324219, 'wood'], [486.95709228515625, 359.95709228515625, 'wood'], [281.0, 110.27731323242188, 'wood'], [279.0, 89.27731323242188, 'wood'], [279.0, 69.27731323242188, 'wood']]
foodList=[[73.5, 339.0, 'food'], [487.0, 330.0, 'food'], [73.5, 309.5, 'food'], [142.0, 44.5, 'food'], [119.0, 44.5, 'food'], [92.0, 44.5, 'food']]
townList=[]
points=[]

cap  =  cv2.VideoCapture(0)
img=cap.read()
while(1):
    a,img=cap.read()
    cv2.imshow("video",img)
    if cv2.waitKey(1) & 0xFF == 27: break
height, width, channels=img.shape
centroid=np.zeros((height,width,channels), np.uint8)
try:
    myfile = open("river.txt","r")
    while(1):
        line=myfile.readline()
        if not line: break
        riverListhere.append(line.split(',')[0],line.split(',')[1])
    myfile.close()

    myfile=open("wood.txt","r")
    while(1):
        line=myfile.readline()
        if not line: break
        woodList.append(line.split(',')[0],line.split(',')[1])
    myfile.close()

    myfile = open("food.txt","r")
    while(1):
        line=myfile.readline()
        if not line: break
        foodList.append(line.split(',')[0],line.split(',')[1])
    myfile.close()

    myfile= open("town.txt","r")
    while(1):
        line=myfile.readline()
        if not line: break
        townList.append(line.split(',')[0],line.split(',')[1])
    myfile.close()
except:
    print "Some file is missing."

sumx=0; sumy=0
for i in townList:
    sumx+=i[0]
    sumy+=i[1]
try:
 town=[int(sumx/len(townList)),int(sumy/len(townList))]
except:
    print "Hey! There's no town!"
    cv2.waitKey(1000)
    town=[191.5, 253.5]
area=int(raw_input("Enter area: "))
draw()
print "drawn"
plot()
print "plotted"
pathPlanning()
i=0; j=1
colorinc=50
while(1):
    if ( (points[j]) == 1): j+=1
    if (j>=len(points)):
        break
    plotLine(points[i][0],points[i][1],points[j][0],points[j][1])
    i=j;j+=1
print "\nPath: "+str(points)
cv2.imshow("Path",centroid)
cv2.waitKey(0)
myfile=open("list.txt","w")
for i in points:
    if type(i) is int:
        myfile.write(str(i)+"\n")
    else:
        myfile.write(str(i[0])+"\n")
        myfile.write(str(i[1])+"\n")
myfile.close()
