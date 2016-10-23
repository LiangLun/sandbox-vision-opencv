/*
找一张图片和一个logo,实现把logo叠加到图片上，类似于水印的效果。
上传实现代码和效果图。
TIPS：使用OpenCV先设置ROI，然后把ROI换成另外一个图片的Mat。
*/

// #include "stdafx.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
using namespace cv;

//int _tmain(int argc, _TCHAR* argv[])
int main(int argc, char* argv[])
{
    Mat image = imread("lena.jpg");
    Mat logo = imread("dataguru_logo.jpg");
    Mat ROIimage = image(cv::Rect(image.cols-logo.cols-10,image.rows-logo.rows-10,logo.cols,logo.rows));
    //Mat mask = imread("F:\\VS2010workplace\\cvTestLoge\\cvTestLoge\\logo.bmp",0);
    //logo.copyTo(ROIimage,mask);
    addWeighted(ROIimage,1.0,logo,0.7,0,ROIimage);
    namedWindow("logo");
    imshow("logo",image);
    waitKey(0);
    return 0;
}
