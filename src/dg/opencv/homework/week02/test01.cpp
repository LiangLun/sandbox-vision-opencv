// #include "stdafx.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char* argv[])
{
    Mat image = imread("lena.jpg");
    Mat logo = imread("dataguru_logo.jpg");
    // printf("image.rows=%d\n",image.row());
    Mat ROIimage = image(cv::Rect(10,10,logo.cols,logo.rows));
    logo.copyTo(ROIimage);
    namedWindow("logo");
    imshow("logo",image);
    waitKey(0);
    return 0;
}
