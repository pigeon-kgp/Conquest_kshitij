#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include<bits/stdc++.h>

using namespace cv;
using namespace std;

int botMove(int pointS[],int pointE[]);
int motorWrite(char d);
int botdetect(Mat image);

int FrontX, FrontY, BackX, BackY;
int midX,midY;
double DCenter,DFront,DBack;

char fileName[20]="/dev/ttyACM0";
double finalAngle;

int pLowH = 0;
int pHighH = 153;
int pLowS = 0;
int pHighS = 255;
int pLowV = 25;
int pHighV = 237;


int oLowH = 0;
int oHighH = 0;
int oLowS = 40;
int oHighS = 67;
int oLowV = 0;
int oHighV = 97;
int distanceThreshold_4_goal=70;
int turningThreshold=20;
int distanceThreshold=70;
Mat image;
/*
double distance(line,point):
    point1=line[0]
    point2=line[1]
    dist=(point2[0]-point1[0])*(point[1]-point1[1])-(point2[1]-point1[1])*(point[0]-point1[0])
    ss=sqrt((point2[1]-point1[1])**2 + (point2[0]-point1[0])**2)
    if not ss==0:
        return float(dist)/ss
    else:
	return 0
*/
int distance(int point1[],int point2[])
{
  try{
    double ss=sqrt(pow(point2[1]-point1[1],2) + pow(point2[0]-point1[0],2));

    DFront=(point2[0]-point1[0])*(FrontY-point1[1])-(point2[1]-point1[1])*(FrontX-point1[0]);
    DCenter=(point2[0]-point1[0])*(midY-point1[1])-(point2[1]-point1[1])*(midX-point1[0]);
    DBack=(point2[0]-point1[0])*(BackY-point1[1])-(point2[1]-point1[1])*(BackX-point1[0]);
    if (ss>0)
    {
      DFront/=ss;
      DCenter/=ss;
      DBack/=ss;
    }
    double theta1,theta2;
    try{
      if (point1[1]==point1[0])
      return 0;
      if (FrontX==BackX)
      return 0;
    theta1=atan((point2[1]-point1[1])/(point1[1]-point1[0]));
    theta2=atan((FrontY-BackY)/(FrontX-BackX));
    }
    catch(...)
    {
      return 0;
    }

    finalAngle = theta2-theta1;
    return 1;
  }
  catch (...)
  {
    return 0;
  }
}
int botMove(int pointS[],int pointE[])
{
  namedWindow("Frame",1);
  char s,w;
  double GoalDistance=INT_MAX;
  int ledBlink=pointE[2];
  //Mat frame;
  VideoCapture cap("abc.webm");
  if(!cap.isOpened())
      return -1;
  for(;;)
  {
      Mat frame;
      cap >> frame;
      //motorWrite('S');
      imshow("Frame",frame);
      if(waitKey(30) >= 0) break;
      if(!botdetect(frame))
        continue;


      line(frame,cvPoint(FrontX,FrontY),cvPoint(midX,midY),CV_RGB(0,255,0),1,8);
      line(frame,cvPoint(midX,midY),cvPoint(BackX,BackY),CV_RGB(0,255,0),1,8);
      //GaussianBlur(edges, edges, Size(7,7), 1.5, 1.5);
      //Canny(edges, edges, 0, 30, 3);
      GoalDistance=sqrt(pow(pointE[1]-midY,2) + pow(pointE[0]-midX,2));
      if (GoalDistance < distanceThreshold_4_goal)
      {
        if (ledBlink)
        {
          cout<<"LED";
          motorWrite('b');
        }
        return 1;
      }
      if(!distance(pointS,pointE))
        continue;
      cout<<DFront<<" "<<" "<<DBack<<" "<<DCenter<<endl<<finalAngle<<endl;
      /*
      DFront=distance(line,front);
      DCenter=distance(line,center);
      DBack=distance(line,back);
      */

      if (FrontX==-1)
        continue;
      if (finalAngle<0)
        {
          s='w';
          w='s';
        }
        else
        {
          w='w';
          s='s';
        }
      if (DBack < DCenter && DCenter < DFront)
      {
          ///cout<<s<<"     ";
          motorWrite(s);
      }
      if( DBack > DCenter && DCenter > DFront )
      {
          //cout<<w<<"     ";
          motorWrite(w);
      }

      if (DBack > DCenter && DCenter< DFront )
      {
          if (abs(DCenter) < distanceThreshold )
          {
              //cout<<w<<"     ";
              motorWrite(w);
          }
          else if( DFront-DBack >= turningThreshold)
          {
            //cout<<"a    ";
            motorWrite('a');
          }
          else if (DBack-DFront >= turningThreshold)
          {
            //cout<<"d     ";
            motorWrite('d');
          }
      }
      imshow("Frame",frame);

      if(waitKey(60) >= 0) break;
  }
return 1;
}








int motorWrite(char d)
{
  fstream files;
  files.open(fileName,ios::out);
  if (files.is_open())
  {
    files <<d;
    files.close();
    cout<<d<<endl;
    return 1;
  }
  else{
  cout<<"0"<<endl;
  return 0;
  }
}

int botdetect(Mat image)
{
  Mat img_copy;
  img_copy=image.clone();
  int height=img_copy.size().height;
  int width =img_copy.size().width;
  medianBlur(img_copy,img_copy, 3);
  Mat HSV,pink_part,orange_part;

  cvtColor(img_copy, HSV, CV_BGR2HSV);
  inRange(HSV, Scalar(5+oLowH, 50+oLowS, 223+oLowV),Scalar(15+oHighH, 118+oHighS, 255+oHighV), orange_part);
  inRange(HSV, Scalar(157+pLowH,70+pLowS , 135+pLowV),Scalar(165+pHighH, 150+pHighS, 221+pHighV), pink_part);
  int fx,fy,f,bx,by,b,flagpink,flagorange;
  fx=fy=f=bx=by=b=flagpink=flagorange=0;
  for(int y=0;y<height;y++)
  {
    for(int x=0;x<width;x++)
    {
      if (pink_part.at<uchar>(y,x)>250)
      {
        flagpink=1;
        fy+=y;
        fx+=x;
        f+=1;
      }
      if (orange_part.at<uchar>(y,x)>250)
      {
        flagorange=1;
        by+=y;
        bx+=x;
        b+=1;
      }
    }
  }
  //cout<<FrontX<<" "<<FrontY<<" "<<BackX<<" "<<BackY<<endl;
  imshow("abc",pink_part+orange_part);
  waitKey(1);
  try
  {
    if(flagpink && flagorange)
    {
      FrontX=fx/f;    FrontY=fy/f;
      BackX=bx/b;     BackY=by/b;
      midX=(FrontX+BackX)/2;
      midY=(FrontY+BackY)/2;
      return 1;
    }
  }
  catch (...)
  {
    return 0;
  }
}




int main(int, char**)
{

    FrontX=FrontY=BackX=BackY=-1;
    /*cout<<"fileName"<<endl;
    char qa;
    cin>>qa;
    fileName[11]=qa;
    cout<<"VideoCapture from"<<endl;
    cin>>qa;*/
    namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"

    cvCreateTrackbar("OrangeLowH", "Control", &oLowH, 179); //Hue (0 - 179)
    cvCreateTrackbar("OrangeHighH", "Control", &oHighH, 179);

    cvCreateTrackbar("OrangeLowS", "Control", &oLowS, 255); //Saturation (0 - 255)
    cvCreateTrackbar("OrangeHighS", "Control", &oHighS, 255);

    cvCreateTrackbar("OrangeLowV", "Control", &oLowV, 255); //Value (0 - 255)
    cvCreateTrackbar("OrangeHighV", "Control", &oHighV, 255);


    cvCreateTrackbar("PinkLowH", "Control", &pLowH, 179); //Hue (0 - 179)
    cvCreateTrackbar("PinkHighH", "Control", &pHighH, 179);

    cvCreateTrackbar("PinkLowS", "Control", &pLowS, 255); //Saturation (0 - 255)
    cvCreateTrackbar("PinkHighS", "Control", &pHighS, 255);

    cvCreateTrackbar("PinkLowV", "Control", &pLowV, 255); //Value (0 - 255)
    cvCreateTrackbar("PinkHighV", "Control", &pHighV, 255);
    int a[]={1,2,3};
    int b[]={800,100,3};
    botMove(a,b);

}
