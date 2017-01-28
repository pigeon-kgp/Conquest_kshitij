#include "opencv2/imgproc/imgproc.hpp" //
#include "opencv2/highgui/highgui.hpp"

    #include <bits/stdc++.h>

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/features2d/features2d.hpp"



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
int bLowV = 55;
int bHighV = 255;


int yLowH = 0;
int yHighH = 179;
int yLowS = 0;
int yHighS = 255;
int yLowV = 0;
int yHighV = 255;

int Area=20;
int MArea=1000;

vector<KeyPoint> rivers,towncenter,resource;
Mat preprocessed;

int Filter(Mat );
int BlobDetect(Mat );
int plotBlob(Mat part,Mat bin,vector<KeyPoint>& keypoints,Mat out);

int main()
{
  namedWindow("Control", CV_WINDOW_AUTOSIZE);
  namedWindow("Control2",CV_WINDOW_AUTOSIZE);
   //create a window called "Control"
  namedWindow("frame");
  namedWindow("preprocessed");
  cvCreateTrackbar("YellowLowH", "Control", &yLowH, 17); //Hue (0 - 179)
  cvCreateTrackbar("YellowHighH", "Control", &yHighH, 17);
  cvCreateTrackbar("YellowLowS", "Control", &yLowS, 255); //Saturation (0 - 255)
  cvCreateTrackbar("YellowHighS", "Control", &yHighS, 255);
  cvCreateTrackbar("YellowLowV", "Control", &yLowV, 255); //Value (0 - 255)
  cvCreateTrackbar("YellowHighV", "Control", &yHighV, 255);


  cvCreateTrackbar("BlueLowH", "Control", &bLowH, 17); //Hue (0 - 179)
  cvCreateTrackbar("BlueHighH", "Control", &bHighH, 19);
  cvCreateTrackbar("BlueLowS", "Control", &bLowS, 255); //Saturation (0 - 255)
  cvCreateTrackbar("BlueHighS", "Control", &bHighS, 255);
  cvCreateTrackbar("BlueLowV", "Control", &bLowV, 255); //Value (0 - 255)
  cvCreateTrackbar("BlueHighV", "Control", &bHighV, 255);


  cvCreateTrackbar("BrownLowH", "Control2", &brLowH, 19); //Hue (0 - 179)
  cvCreateTrackbar("BrownHighH", "Control2", &brHighH, 17);
  cvCreateTrackbar("BrownLowS", "Control2", &brLowS, 255); //Saturation (0 - 255)
  cvCreateTrackbar("BrownHighS", "Control2", &brHighS, 255);
  cvCreateTrackbar("BrownLowV", "Control2", &brLowV, 255); //Value (0 - 255)
  cvCreateTrackbar("BrownHighV", "Control2", &brHighV, 255);

  cvCreateTrackbar("Area", "Control", &Area, 1500);
  cvCreateTrackbar("MArea", "Control", &MArea, 1500);

  char c;
  const char* fileName ="abc.webm";
  VideoCapture cap(fileName); //
  if(!cap.isOpened()) //
  {
      cout << "Couldn't open Video  " << fileName << "\n";
      return -1;
  }
  for(;;)  // videonun frameleri icin sonsuz dongu
  {

      Mat frame,labelImg;
      cap >> frame;
      //preprocessed=Mat::zeros(frame.size(),CV_8U);
      preprocessed=frame.clone();
      imshow("frame",frame);
      waitKey(40);
      if(frame.empty()) break;
      BlobDetect(frame);
      //c=waitKey(10);
      if ((cvWaitKey(40)&0xff)==32)
      {
        cout<<"Yo Liked the Image subscribe it ";
        char abc;cin>>abc;
        if ((abc=='y' || abc=='Y'))
        {
          fstream RIVER,FOOD,WOOD,TOWN;
          //cout<<"insuidedewaxa"<<rivers.size()<<endl<<endl;
          RIVER.open("river.txt",ios::out);
          FOOD.open("food.txt",ios::out);
          WOOD.open("wood.txt",ios::out);
          TOWN.open("town.txt",ios::out);
          //waitKey(0);

          for (int i=0;i<rivers.size();i++)
            RIVER<<rivers[i].pt.x<<","<<rivers[i].pt.y<<endl;
          /*for (int i=0;i<food.size();i++)
            FOOD<<food[i].pt.x<<","<<food[i].pt.y<<endl;
          for (int i=0;i<wood.size();i++)
            WOOD<<wood[i].pt.x<<","<<wood[i].pt.y<<endl;*/
          for (int i=0;i<rivers.size();i++)
            TOWN<<towncenter[i].pt.x<<","<<towncenter[i].pt.y<<endl;
        }
      }
    }



}

int Filter(Mat HSV)
{
  Mat hsv;
  hsv=HSV.clone();


  int height=hsv.size().height;
  int width =hsv.size().width;

  Mat yellow_bin,blue_bin,brown_bin,blue_part,yellow_part,brown_part;
  Mat out_blue,out_yellow,out_brown;
  inRange(HSV, Scalar(16+yLowH, 90+yLowS, 116+yLowV),Scalar(30+yHighH, 138+yHighS, 255+yHighV), yellow_part);
  inRange(HSV, Scalar(92+bLowH,41+bLowS , 79+bLowV),Scalar(109+bHighH, 58+bHighS, 255+bHighV), blue_part);
  inRange(HSV, Scalar(1+brLowH,70+brLowS , 120+brLowV),Scalar(12+brHighH, 114+brHighS, 129+brHighV), brown_part);

  threshold(yellow_part,yellow_bin,120,255,CV_THRESH_BINARY);
  threshold(brown_part,brown_bin,120,255,CV_THRESH_BINARY);
  threshold(blue_part,blue_bin,120,255,CV_THRESH_BINARY);

  plotBlob(blue_part,blue_bin,rivers,out_blue);
  plotBlob(yellow_part,yellow_bin,resource,out_yellow);
  plotBlob(brown_part,brown_bin,towncenter,out_brown);
  //cv::imshow("Blobs", out_yellow);
  //waitKey(40);



  imshow("preprocessed",preprocessed);
  waitKey(40);

  /*
  int flagpink,flagorange;
  flagpink=flagorange=0;
  for(int y=0;y<height;y++)
  {
    for(int x=0;x<width;x++)
    {
      if (yellow_part.at<uchar>(y,x)>250)
      {
        hsv.at<Vec3b>(y,x)[0]=10;
        hsv.at<Vec3b>(y,x)[1]=255;
        hsv.at<Vec3b>(y,x)[2]=255;
      }
      if (blue_part.at<uchar>(y,x)>250)
      {
        hsv.at<Vec3b>(y,x)[0]=70;
        hsv.at<Vec3b>(y,x)[1]=255;
        hsv.at<Vec3b>(y,x)[2]=255;
      }
      if (brown_part.at<uchar>(y,x)>250)
      {
        hsv.at<Vec3b>(y,x)[0]=110;
        hsv.at<Vec3b>(y,x)[1]=255;
        hsv.at<Vec3b>(y,x)[2]=255;
      }
    }
  }
  return hsv;
  */
}
int BlobDetect(Mat image)
{
  Mat img_copy;
  img_copy=image.clone();
  int height=img_copy.size().height;
  int width =img_copy.size().width;
  medianBlur(img_copy,img_copy, 3);
  medianBlur(img_copy,img_copy, 3);
  medianBlur(img_copy,img_copy, 3);
  medianBlur(img_copy,img_copy, 3);
  Mat HSV;
  cvtColor(img_copy, HSV, CV_BGR2HSV);
  Filter(HSV);
}

int plotBlob(Mat part,Mat bin,vector<KeyPoint>& keypoints,Mat out)
{
  SimpleBlobDetector::Params params;
  params.minThreshold = 120;
  params.maxThreshold = 255;
  params.thresholdStep = 100;

  params.minArea = Area;
  params.minConvexity = 0.3;
  params.minInertiaRatio = 0.01;

  params.maxArea = MArea;
  params.maxConvexity = 10;

  params.filterByColor = false;
  params.filterByCircularity = false;

  //blue_src = blue_bin.clone();
  SimpleBlobDetector blob_detector(params);
  // or cv::Ptr<cv::SimpleBlobDetector> detector = cv::SimpleBlobDetector::create(params)

  // detect!
  vector<KeyPoint> keypoints2;
  blob_detector.detect(bin, keypoints2);//drawKeypoints( bin, keypoints, out, CV_RGB(255,0,0), DrawMatchesFlags::DEFAULT);


  //Mat blobImg;
  cout<<keypoints2.size()<<endl;
  //drawKeypoints(part, keypoints, blobImg);
  keypoints=keypoints2;
  for(int i=0;i<keypoints2.size();i++)
  {
    preprocessed.at<Vec3b>(keypoints2[i].pt.y,keypoints2[i].pt.x)[0]=30;
    preprocessed.at<Vec3b>(keypoints2[i].pt.y+1,keypoints2[i].pt.x+1)[0]=30;
    preprocessed.at<Vec3b>(keypoints2[i].pt.y+1,keypoints2[i].pt.x)[0]=30;
    preprocessed.at<Vec3b>(keypoints2[i].pt.y,keypoints2[i].pt.x+1)[0]=30;
  }
}
