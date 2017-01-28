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



int yLowH = 0;
int yHighH = 179;
int yLowS = 0;
int yHighS = 255;
int yLowV = 0;
int yHighV = 255;

int MinAreaW=200;
int MaxAreaW=1000;

int MinAreaF=20;
int MaxAreaF=1000;

vector<KeyPoint> food,towncenter,wood;
Mat preprocessed;  Mat HSV;Mat temp_frame;

int Filter(Mat );
int BlobDetect(Mat );
int plotBlob(Mat part,Mat bin,vector<KeyPoint>& keypoints,Mat out,int ,int);

void on_trackbar( int, void* )
{
  preprocessed=temp_frame.clone();
  Filter(HSV);
}

int plotBlob(Mat part,Mat bin,vector<KeyPoint>& keypoints,Mat out,int MinArea=20,int MaxArea=1000)
{
  SimpleBlobDetector::Params params;
  params.minThreshold = 120;
  params.maxThreshold = 255;
  params.thresholdStep = 100;

  params.minArea = MinArea;
  params.minConvexity = 0.3;
  params.minInertiaRatio = 0.01;

  params.maxArea = MaxArea;
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
    preprocessed.at<Vec3b>(keypoints2[i].pt.y,keypoints2[i].pt.x)[0]=255;
    preprocessed.at<Vec3b>(keypoints2[i].pt.y+1,keypoints2[i].pt.x+1)[0]=255;
    preprocessed.at<Vec3b>(keypoints2[i].pt.y+1,keypoints2[i].pt.x)[0]=255;
    preprocessed.at<Vec3b>(keypoints2[i].pt.y,keypoints2[i].pt.x+1)[0]=255;
  }
}

int Filter(Mat HSV)
{
  Mat hsv;
  hsv=HSV.clone();


  int height=hsv.size().height;
  int width =hsv.size().width;

  Mat yellow_bin,brown_bin,yellow_part,brown_part;
  Mat out_yellow,out_brown;
  inRange(HSV, Scalar(16+yLowH, 90+yLowS, 116+yLowV),Scalar(30+yHighH, 138+yHighS, 255+yHighV), yellow_part);
  inRange(HSV, Scalar(1+brLowH,70+brLowS , 120+brLowV),Scalar(12+brHighH, 114+brHighS, 129+brHighV), brown_part);

  threshold(yellow_part,yellow_bin,120,255,CV_THRESH_BINARY);
  threshold(brown_part,brown_bin,120,255,CV_THRESH_BINARY);
  imshow("foddd +wood",yellow_part);
  waitKey(1);

  plotBlob(yellow_part,yellow_bin,wood,out_yellow,MinAreaW,MaxAreaW);
  plotBlob(yellow_part,yellow_bin,food,out_yellow,MinAreaF,MaxAreaF);
  plotBlob(brown_part,brown_bin,towncenter,out_brown);
  //cv::imshow("Blobs", out_yellow);
  //waitKey(40);
  for (int i=0;i<food.size();i++)
  {
    putText(preprocessed, "F", Point(food[i].pt.x, food[i].pt.y), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0), 2.0);
  }

  for (int i=0;i<wood.size();i++)
  {
    putText(preprocessed, "W", Point(wood[i].pt.x, wood[i].pt.y), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0), 2.0);
  }


  imshow("preprocessed",preprocessed);
  waitKey(40);

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

  cvtColor(img_copy, HSV, CV_BGR2HSV);
  Filter(HSV);
}

int main()
{
  namedWindow("Control", CV_WINDOW_AUTOSIZE);
  namedWindow("Control2",CV_WINDOW_AUTOSIZE);
  namedWindow("Control3",CV_WINDOW_AUTOSIZE);
   //create a window called "Control"
  namedWindow("frame",WINDOW_NORMAL);
  namedWindow("preprocessed",WINDOW_NORMAL);
  cvCreateTrackbar("YellowLowH", "Control", &yLowH, 17); //Hue (0 - 179)
  cvCreateTrackbar("YellowHighH", "Control", &yHighH, 17);
  cvCreateTrackbar("YellowLowS", "Control", &yLowS, 255); //Saturation (0 - 255)
  cvCreateTrackbar("YellowHighS", "Control", &yHighS, 255);
  cvCreateTrackbar("YellowLowV", "Control", &yLowV, 255); //Value (0 - 255)
  cvCreateTrackbar("YellowHighV", "Control", &yHighV, 255);



  cvCreateTrackbar("BrownLowH", "Control2", &brLowH, 19); //Hue (0 - 179)
  cvCreateTrackbar("BrownHighH", "Control2", &brHighH, 17);
  cvCreateTrackbar("BrownLowS", "Control2", &brLowS, 255); //Saturation (0 - 255)
  cvCreateTrackbar("BrownHighS", "Control2", &brHighS, 255);
  cvCreateTrackbar("BrownLowV", "Control2", &brLowV, 255); //Value (0 - 255)
  cvCreateTrackbar("BrownHighV", "Control2", &brHighV, 255);

  cvCreateTrackbar("MinAreaF", "Control3", &MinAreaF, 1500);
  cvCreateTrackbar("MaxAreaF", "Control3", &MaxAreaF, 1500);

  cvCreateTrackbar("MinAreaW", "Control3", &MinAreaW, 1500);
  cvCreateTrackbar("MaxAreaW", "Control3", &MaxAreaW, 1500);

  char c;
  const char* fileName ="conq.webm";
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
      temp_frame=frame.clone();
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
          fstream FOOD,WOOD,TOWN;
          //cout<<"insuidedewaxa"<<rivers.size()<<endl<<endl;

          FOOD.open("food.txt",ios::out);
          WOOD.open("wood.txt",ios::out);
          TOWN.open("town.txt",ios::out);
          //waitKey(0);


          for (int i=0;i<food.size();i++)
            FOOD<<food[i].pt.x<<","<<food[i].pt.y<<endl;
          for (int i=0;i<wood.size();i++)
            WOOD<<wood[i].pt.x<<","<<wood[i].pt.y<<endl;
          for (int i=0;i<towncenter.size();i++)
            TOWN<<towncenter[i].pt.x<<","<<towncenter[i].pt.y<<endl;
        }
      }
    }



}
