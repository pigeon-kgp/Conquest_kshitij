#include "opencv2/imgproc/imgproc.hpp" //
#include "opencv2/highgui/highgui.hpp"

    #include <iostream>
    #include <math.h>
    #include <vector>
    #include <fstream>
    #include <string>
    #include <sstream>
    #include <algorithm>

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/features2d/features2d.hpp"


using namespace cv;
using namespace std;


int main(int argc, char *argv[])
{


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
        if(frame.empty()) break;
        //imshow("main",frame);

        Mat frame_gray;
        //cvtColor(frame,frame_gray,CV_RGB2GRAY);

       medianBlur(frame,frame, 3);
        //////////////////////////////////////////////////////////////////////////
        // convert binary_image
        Mat binaryx;
        threshold(frame,binaryx,120,255,CV_THRESH_BINARY);


        Mat src, gray, thresh, binary;
        Mat out;
        vector<KeyPoint> keyPoints;

        SimpleBlobDetector::Params params;
        params.minThreshold = 120;
        params.maxThreshold = 255;
        params.thresholdStep = 100;

        params.minArea = 20;
        params.minConvexity = 0.3;
        params.minInertiaRatio = 0.01;

        params.maxArea = 1000;
        params.maxConvexity = 10;

        params.filterByColor = false;
        params.filterByCircularity = false;



        src = binaryx.clone();

        SimpleBlobDetector blobDetector( params );
        blobDetector.create("SimpleBlob");



        blobDetector.detect( src, keyPoints );
        drawKeypoints( src, keyPoints, out, CV_RGB(255,0,0), DrawMatchesFlags::DEFAULT);


        cv::Mat blobImg;
        cv::drawKeypoints(frame, keyPoints, blobImg);
        cv::imshow("Blobs", blobImg);

        for(int i=0; i<keyPoints.size(); i++){
            //circle(out, keyPoints[i].pt, 20, cvScalar(255,0,0), 10);
            //cout<<keyPoints[i].response<<endl;
            //cout<<keyPoints[i].angle<<endl;
            //cout<<keyPoints[i].size()<<endl;
            cout<<keyPoints[i].pt.x<<endl;
            cout<<keyPoints[i].pt.y<<endl;

        }
        imshow( "out", out );

        if ((cvWaitKey(40)&0xff)==27) break;  // esc 'ye basilinca break
    }
    system("pause");

}
