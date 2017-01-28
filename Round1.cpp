#include "opencv2/imgproc/imgproc.hpp" //
#include "opencv2/highgui/highgui.hpp"

    #include <bits/stdc++.h>

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/features2d/features2d.hpp"



using namespace cv;
using namespace std;


//int yhL=14,ysL=80, yvL=106,yhH=18,ysH=100, yvH=126,bhL=1,bsL=60, bvL=110,bhH=3,bsH=80, bvH=130;
int yhL=255,ysL=255, yvL=255,yhH=0,ysH=0, yvH=0,bhL=1,bsL=60, bvL=110,bhH=3,bsH=80, bvH=130;

int rect[4][2];

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

int AreaCut=1000;
int AreaCut2=1000;

vector<KeyPoint> food,towncenter,wood;
Mat preprocessed;  Mat HSV;Mat temp_frame; Mat hsv;

int Filter(Mat );
int BlobDetect(Mat );
int plotBlob(Mat part,Mat bin,vector<KeyPoint>& keypoints,Mat out,int ,int);

void onClick(int event, int x, int y, int flags, void* userdata)
{
  Mat img;
  cvtColor(::preprocessed, img, CV_BGR2HSV);
	if (event == EVENT_LBUTTONDOWN)
  {
		printf("%d %d: ", y,x);
		cout << img.at<Vec3b>(y, x) << endl;
    if (img.at<Vec3b>(y, x)[0]<(::yhL))
      ::yhL=img.at<Vec3b>(y, x)[0];
    if (img.at<Vec3b>(y, x)[0]>(::yhH))
      ::yhH=img.at<Vec3b>(y, x)[0];
    if (img.at<Vec3b>(y, x)[1]<(::ysL))
      ::ysL=img.at<Vec3b>(y, x)[1];
    if (img.at<Vec3b>(y, x)[1]>(::ysH))
      ::ysH=img.at<Vec3b>(y, x)[1];
    if (img.at<Vec3b>(y, x)[2]<(::yvL))
      ::yvL=img.at<Vec3b>(y, x)[2];
    if (img.at<Vec3b>(y, x)[2]>(::yvL))
      ::yvH=img.at<Vec3b>(y, x)[2];
    cout<<endl<<"LowY: "<<::yhL<<" "<<::ysL<<" "<<::yvL<<endl;
    cout<<endl<<"HighY: "<<::yhH<<" "<<::ysH<<" "<<::yvH<<endl;
  }
  if (event == EVENT_RBUTTONDOWN)
  {
    printf("%d %d: ", y,x);
    cout << img.at<Vec3b>(y, x) << endl;
    if (img.at<Vec3b>(y, x)[0]<(::bhL))
      ::bhL=img.at<Vec3b>(y, x)[0];
    if (img.at<Vec3b>(y, x)[0]>(::bhH))
      ::bhH=img.at<Vec3b>(y, x)[0];
    if (img.at<Vec3b>(y, x)[1]<(::bsL))
      ::bsL=img.at<Vec3b>(y, x)[1];
    if (img.at<Vec3b>(y, x)[1]>(::bsH))
      ::bsH=img.at<Vec3b>(y, x)[1];
    if (img.at<Vec3b>(y, x)[2]<(::bvL))
      ::bvL=img.at<Vec3b>(y, x)[2];
    if (img.at<Vec3b>(y, x)[2]>(::bvL))
      ::bvH=img.at<Vec3b>(y, x)[2];
    cout<<endl<<"LowB: "<<::bhL<<" "<<::bsL<<" "<<::bvL<<endl;
    cout<<endl<<"HighB: "<<::bhH<<" "<<::bsH<<" "<<::bvH<<endl;
  }
}
int count=0;
void onClick_rec(int event, int x, int y, int flags, void* userdata)
{
	if (event == EVENT_LBUTTONDOWN)
  {
    rect[::count][0]=x;
    rect[::count][1]=y;
    ::count++;
    cout<<"\nSelected.\n";
    waitKey(0);
  }
}

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
  inRange(HSV, Scalar(::yhL,::ysL, ::yvL),Scalar(::yhH,::ysH, ::yvH), yellow_part);
  inRange(HSV, Scalar(::bhL,::bsL, ::bvL),Scalar(::bhH,::bsH, ::bvH), brown_part);

  threshold(yellow_part,yellow_bin,120,255,CV_THRESH_BINARY);
  threshold(brown_part,brown_bin,120,255,CV_THRESH_BINARY);
  imshow("foddd +wood",yellow_part);
  waitKey(1);

  plotBlob(yellow_part,yellow_bin,wood,out_yellow,1,AreaCut);
  plotBlob(yellow_part,yellow_bin,food,out_yellow,AreaCut2,5000);
  plotBlob(brown_part,brown_bin,towncenter,out_brown);
  //cv::imshow("Blobs", out_yellow);
  //waitKey(40);
  for (int i=0;i<food.size();i++)
  {
    int x=food[i].pt.x; int y=food[i].pt.y;
    if (((x>=rect[0][0] || x>=rect[3][0])&&(x<=rect[1][0] || x<=rect[2][0])) && ((y>=rect[0][1] || y>=rect[1][1])&&(y<=rect[2][1] || y<=rect[3][1])))
    putText(preprocessed, "F", Point(food[i].pt.x, food[i].pt.y), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0), 2.0);
  }

  for (int i=0;i<wood.size();i++)
  {
    int x=wood[i].pt.x; int y=wood[i].pt.y;
    if (((x>=rect[0][0] || x>=rect[3][0])&&(x<=rect[1][0] || x<=rect[2][0])) && ((y>=rect[0][1] || y>=rect[1][1])&&(y<=rect[2][1] || y<=rect[3][1])))
    putText(preprocessed, "W", Point(wood[i].pt.x, wood[i].pt.y), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0), 2.0);
  }

  for (int i=0;i<towncenter.size();i++)
  {
    int x=towncenter[i].pt.x; int y=towncenter[i].pt.y;
    if (((x>=rect[0][0] || x>=rect[3][0])&&(x<=rect[1][0] || x<=rect[2][0])) && ((y>=rect[0][1] || y>=rect[1][1])&&(y<=rect[2][1] || y<=rect[3][1])))
    putText(preprocessed, "T", Point(x, y), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0), 2.0);
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
  namedWindow("choose",WINDOW_NORMAL);
  cvCreateTrackbar("YellowLowH", "Control", &yLowH, 17); //Hue (0 - 179)
  cvCreateTrackbar("YellowHighH", "Control", &yHighH, 17);
  cvCreateTrackbar("YellowLowS", "Control", &yLowS, 255); //Saturation (0 - 255)
  cvCreateTrackbar("YellowHighS", "Control", &yHighS, 255);
  cvCreateTrackbar("YellowLowV", "Control", &yLowV, 255); //Value (0 - 255)
  cvCreateTrackbar("YellowHighV", "Control", &yHighV, 255);
  setMouseCallback("preprocessed", onClick, NULL);
  setMouseCallback("choose", onClick_rec, NULL);


  cvCreateTrackbar("BrownLowH", "Control2", &brLowH, 19); //Hue (0 - 179)
  cvCreateTrackbar("BrownHighH", "Control2", &brHighH, 17);
  cvCreateTrackbar("BrownLowS", "Control2", &brLowS, 255); //Saturation (0 - 255)
  cvCreateTrackbar("BrownHighS", "Control2", &brHighS, 255);
  cvCreateTrackbar("BrownLowV", "Control2", &brLowV, 255); //Value (0 - 255)
  cvCreateTrackbar("BrownHighV", "Control2", &brHighV, 255);

  cvCreateTrackbar("Area cutoff max", "Control3", &AreaCut, 5000);
  cvCreateTrackbar("Area cutoff min", "Control3", &AreaCut2, 5000);

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
      cout<<frame.size();
      //preprocessed=Mat::zeros(frame.size(),CV_8U);
      preprocessed=frame.clone();
      temp_frame=frame.clone();
      imshow("frame",frame);
      imshow("choose",frame);
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
          {
              int x=food[i].pt.x; int y=food[i].pt.y;
              if (((x>=rect[0][0] || x>=rect[3][0])&&(x<=rect[1][0] || x<=rect[2][0])) && ((y>=rect[0][1] || y>=rect[1][1])&&(y<=rect[2][1] || y<=rect[3][1])))
                FOOD<<food[i].pt.x<<","<<food[i].pt.y<<endl;
          }
          for (int i=0;i<wood.size();i++)
          {
            int x=wood[i].pt.x; int y=wood[i].pt.y;
            if (((x>=rect[0][0] || x>=rect[3][0])&&(x<=rect[1][0] || x<=rect[2][0])) && ((y>=rect[0][1] || y>=rect[1][1])&&(y<=rect[2][1] || y<=rect[3][1])))
              WOOD<<wood[i].pt.x<<","<<wood[i].pt.y<<endl;
         }
          for (int i=0;i<towncenter.size();i++)
          {
            int x=towncenter[i].pt.x; int y=towncenter[i].pt.y;
            if (((x>=rect[0][0] || x>=rect[3][0])&&(x<=rect[1][0] || x<=rect[2][0])) && ((y>=rect[0][1] || y>=rect[1][1])&&(y<=rect[2][1] || y<=rect[3][1])))
            TOWN<<towncenter[i].pt.x<<","<<towncenter[i].pt.y<<endl;
          }
        }
      }
    }



}
