#include "stdafx.h"
#include <opencv2/opencv.hpp>
using namespace cv;

int _tmain(int argc, _TCHAR* argv[])
{
    Mat image = imread("lena.jpg");
    Mat logo = imread("dataguru_logo.jpg");
    Mat ROIimage = image(cv::Rect(385,270,logo.cols,logo.rows));
    Mat mask = imread("F:\\VS2010workplace\\cvTestLoge\\cvTestLoge\\logo.bmp",0);
    logo.copyTo(ROIimage,mask);
    namedWindow("logo");
    imshow("logo",image);
    waitKey(0);
    return 0;
}
