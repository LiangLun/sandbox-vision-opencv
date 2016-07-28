#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/ml/ml.hpp>

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
    //Create Mat for the trainig set and classes
    Mat classes(5, 1, CV_32FC1);
    Mat colors(5, 3, CV_32FC1);

    //Training set (primary colors)
    colors.at<float>(0,0)=0, colors.at<float>(0,1)=0, colors.at<float>(0,2)=0;
    colors.at<float>(1,0)=255, colors.at<float>(1,1)=255, colors.at<float>(1,2)=255;
    colors.at<float>(2,0)=255, colors.at<float>(2,1)=0, colors.at<float>(2,2)=0;
    colors.at<float>(3,0)=0, colors.at<float>(3,1)=255, colors.at<float>(3,2)=0;
    colors.at<float>(4,0)=0, colors.at<float>(4,1)=0, colors.at<float>(4,2)=255;

    //Set clases to each training sample
    classes.at<float>(0,0)=1;
    classes.at<float>(1,0)=2;
    classes.at<float>(2,0)=3;
    classes.at<float>(3,0)=4;
    classes.at<float>(4,0)=5;

    //KNN classifier (k=1)
    CvKNearest classifier;
    classifier.train(colors,classes,Mat(),false,1,false);

    //Load original image
    Mat src=imread("baboon.jpg",1);
    imshow("baboon",src);

    //Create result image
    Mat dst(src.rows , src.cols, CV_8UC3);

    Mat results;
    Mat newPoint(1,3,CV_32FC1);

    //Response for each pixel and store the result in the result image
    float prediction=0;
    for(int y = 0; y < src.rows; ++y){
      for(int x = 0; x < src.cols; ++x){
         newPoint.at<float>(0,0)= src.at<Vec3b>(y, x)[0];
         newPoint.at<float>(0,1) = src.at<Vec3b>(y, x)[1];
         newPoint.at<float>(0,2) = src.at<Vec3b>(y, x)[2];
         prediction=classifier.find_nearest(newPoint,1,&results, 0, 0);
         dst.at<Vec3b>(y, x)[0]= colors.at<float>(prediction-1,0);
         dst.at<Vec3b>(y, x)[1]= colors.at<float>(prediction-1,1);
         dst.at<Vec3b>(y, x)[2]= colors.at<float>(prediction-1,2);
      }
    }

    //Show result image
    cv::imshow("result KNN",dst);
    cv::waitKey(0);
    return 0;
}




