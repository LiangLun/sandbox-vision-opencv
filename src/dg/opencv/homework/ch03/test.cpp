// #include "stdafx.h"
/*
1. 在“作业素材”中，有一张horse_hw.jpg的图片，对图片中做目标分割，生成分割后的图片（黑白图）。
2. 在horse_hw.jpg图像内画一个换色的矩形框，在矩形框内用白色写上自己的名字。
3. 把1和2的两张图片上下摆放到图一个输出显示框内。
提交方式：提交最终的效果图片和代码。
TIP：
1和3 在实例2中都有体现如何实现, 2主要使用函数rectangle(),putText()。
*/
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
