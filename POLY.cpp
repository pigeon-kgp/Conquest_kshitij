#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <bits/stdc++.h>

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/features2d/features2d.hpp"

using namespace cv;
using namespace std;

int angthresh=15,distthresh=15;
int FrontX, FrontY, BackX, BackY;
int midX,midY, tarX, tarY;
fstream reader;
double DCenter,DFront,DBack;
char fileName[20]="/dev/ttyACM0";

int FhL=255,FsL=255, FvL=255,FhH=0,FsH=0, FvH=0,BhL=1,BsL=60, BvL=110,BhH=3,BsH=80, BvH=130;
int botdetect(Mat image)
{
  Mat img_copy;
  img_copy=image.clone();
  int height=img_copy.size().height;
  int width =img_copy.size().width;
  medianBlur(img_copy,img_copy, 3);
  Mat HSV,pink_part,orange_part;

  cvtColor(img_copy, HSV, CV_BGR2HSV);
  inRange(HSV, Scalar(BhL,BsL,BvL),Scalar(BhH,BsH,BvH), orange_part);
  inRange(HSV, Scalar(FhL,FsL , FvL),Scalar(FhH, FsH, FvH), pink_part);
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

void Dest (int target[], int midX, int midY)
{
  float dist=sqrt(pow((tarX-midX),2)+pow((tarY-midY),2));
  if (dist>=15)
  {
    target[0]=tarX; target[1]=tarY; return;
  }
  else
  {
    int b,bn;
    reader>>b;
    if (b==1) {target[0]=target[1]=-1; return;}
    else
      reader>>bn;
    target[0]=b; target[1]=bn;
    return;
  }
}

int main()
{
  fstream FRONT, BACK;
  reader.open("list.txt",ios::in);
  reader>>tarX>>tarY;
  try{
    FRONT.open("front.txt", ios::in);
  }
  catch(...)
  {
    cout<<"\nFile front does not exist.\n";
  }
  try{
  BACK.open("back.txt",ios::in);
  }
  catch(...)
  {
    cout<<"\nFile back does not exist.\n";
  }
  FRONT>>::FhL>>::FsL>>::FvL>>::FhH>>::FsH>>::FvH;
  BACK>>::BhL>>::BsL>>::BvL>>::BhH>>::BsH>>::BvH;

  VideoCapture cap(0);
  if(!cap.isOpened())
      return -1;
  for(;;)
  {
      Mat frame;
      cap >> frame;
      //motorWrite('S');
      if(!botdetect(frame))
        continue;
      line(frame,cvPoint(FrontX,FrontY),cvPoint(midX,midY),CV_RGB(0,255,0),1,8);
      line(frame,cvPoint(midX,midY),cvPoint(BackX,BackY),CV_RGB(0,255,0),1,8);
      imshow("Frame",frame);
      if(waitKey(30) >= 0) break;
      float mbot,m;
      try{
        float mbot=float(FrontY-BackY)/(FrontX-BackX);
      }
      catch(...)
      {
        float mbot=999999.0;
      }
      int target[2];
      Dest(target,midX,midY);
      cout<<target[0]<<","<<target[1]<<endl;
      char comm;
      if (target[0]==-1 && target[1]==-1)
        comm='b';
      else
      {
        try{
          float m=float(target[1]-midY)/(target[0]-midX);
        }
        catch(...)
        {
          float m=999999.0;
        }
        float anglebot=atan(mbot)*180/3.142;
        float angle=atan(m)*180/3.142;
        if (anglebot-angle>=angthresh)
          comm='d';
        else if (angle-anglebot>=angthresh)
                comm='a';
             else
             {
                //angthresh=15;
                comm='w';
             }
      }
      fstream files;
      files.open(fileName,ios::out);
      if (files.is_open())
      {
        files<<comm;
        files.close();
        cout<<comm<<endl;
        return 1;
      }
      else
        cout<<"0"<<endl;
    }
}
