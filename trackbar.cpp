#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include<limits.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

using namespace cv;
using namespace std;

int brLowH = 0;
int brHighH = 179;
int brLowS = 0;
int brHighS = 255;
int brLowV = 0;
int brHighV = 255;

int bLowH = 0;
int bHighH = 179;
int bLowS = 0;
int bHighS = 255;
int bLowV = 0;
int bHighV = 255;


int yLowH = 0;
int yHighH = 179;
int yLowS = 0;
int yHighS = 255;
int yLowV = 0;
int yHighV = 255;

int area=0;

int main()
{
  namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"

  cvCreateTrackbar("YellowLowH", "Control", &yLowH, 179); //Hue (0 - 179)
  cvCreateTrackbar("YellowHighH", "Control", &yHighH, 179);
  cvCreateTrackbar("YellowLowS", "Control", &yLowS, 255); //Saturation (0 - 255)
  cvCreateTrackbar("YellowHighS", "Control", &yHighS, 255);
  cvCreateTrackbar("YellowLowV", "Control", &yLowV, 255); //Value (0 - 255)
  cvCreateTrackbar("YellowHighV", "Control", &yHighV, 255);


  cvCreateTrackbar("BlueLowH", "Control", &bLowH, 179); //Hue (0 - 179)
  cvCreateTrackbar("BlueHighH", "Control", &bHighH, 179);
  cvCreateTrackbar("BlueLowS", "Control", &bLowS, 255); //Saturation (0 - 255)
  cvCreateTrackbar("BlueHighS", "Control", &bHighS, 255);
  cvCreateTrackbar("BlueLowV", "Control", &bLowV, 255); //Value (0 - 255)
  cvCreateTrackbar("BlueHighV", "Control", &bHighV, 255);


  cvCreateTrackbar("BrownLowH", "Control", &brLowH, 179); //Hue (0 - 179)
  cvCreateTrackbar("BrownHighH", "Control", &brHighH, 179);
  cvCreateTrackbar("BrownLowS", "Control", &brLowS, 255); //Saturation (0 - 255)
  cvCreateTrackbar("BrownHighS", "Control", &brHighS, 255);
  cvCreateTrackbar("BrownLowV", "Control", &brLowV, 255); //Value (0 - 255)
  cvCreateTrackbar("BrownHighV", "Control", &brHighV, 255);

  cvCreateTrackbar("Area", "Control", &area, 300);


}
Mat* Filter(Mat *HSV)
{
  Mat hsv;
  hsv=HSV.clone();
  inRange(HSV, Scalar(5+oLowH, 50+oLowS, 223+oLowV),Scalar(15+oHighH, 118+oHighS, 255+oHighV), orange_part);
  inRange(HSV, Scalar(157+pLowH,70+pLowS , 135+pLowV),Scalar(165+pHighH, 150+pHighS, 221+pHighV), pink_part);
  inRange(HSV, Scalar(157+pLowH,70+pLowS , 135+pLowV),Scalar(165+pHighH, 150+pHighS, 221+pHighV), pink_part);
  int fx,fy,f,bx,by,b,flagpink,flagorange;
  fx=fy=f=bx=by=b=flagpink=flagorange=0;
  for(int y=0;y<height;y++)
  {
    for(int x=0;x<width;x++)
    {
      if (pink_part[y,x]<5)
      {
        hsv[y,x]=;
      }
      if (orange_part[y,x]<5)
      {
        flagorange=1
        by+=y;
        bx+=x;
        b+=1
      }
    }
  }
}
int BlobDetect(Mat *image)
{
  Mat img_copy;
  img_copy=image.clone();
  height=img_copy.size().height;
  width =img_copy.size().width;
  medianBlur(img_copy,img_copy, 3);
  Mat HSV;
  cvtColor(img_copy, HSV, CV_BGR2HSV);
  Filter(HSV);

}
