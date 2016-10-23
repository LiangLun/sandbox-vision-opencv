#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
using namespace cv;
using namespace std;

/// 全局变量
Mat src;
Mat srcGray, srcCopy;	//srcCopy用于绘图，srcGray用于检测角点
Mat dstHarris;	//dstHarris用于存储角点检测的结果
Mat resHarris;	//resHarris用于存储特征点选择后的结果
/// 各类阈值
int HarrisQualityLevel = 50;
int maxQualityLevel = 100;
double HarrisMinVal = 0.0;
double HarrisMaxVal = 0.0;
const char* HarrisWindow = "My Harris corner detector";

/// 角点检测函数声明
void HarrisFunction( int, void* );

int main( int argc, char** argv )
{
	/// 载入图像并灰度化
	src = imread("./house_small.jpg", 1 );
	cvtColor( src, srcGray, CV_BGR2GRAY );

	/// 设置参数
	int blockSize = 3;
	int apertureSize = 3;

	/// 使用cornerEigenValsAndVecs()函数检测角点
	dstHarris = Mat::zeros( srcGray.size(), CV_32FC(6) );
	resHarris = Mat::zeros( srcGray.size(), CV_32FC1 );
	cornerEigenValsAndVecs( srcGray, dstHarris, blockSize, apertureSize, BORDER_DEFAULT );

	/// 特征点选择
	for( int j = 0; j < srcGray.rows; j++ )
	{
		for( int i = 0; i < srcGray.cols; i++ )
		{
			// 两个特征值
			float* lambda = dstHarris.ptr<float>( j, i);
			float lambda1 = lambda[0];
			float lambda2 = lambda[1];
			resHarris.at<float>(j,i) = lambda1*lambda2 - 0.04*pow( ( lambda1 + lambda2 ), 2 );
		}
  }
  minMaxLoc( resHarris, &HarrisMinVal, &HarrisMaxVal, 0, 0, Mat() );
  
  /// 创建窗口和滑动条
  namedWindow( HarrisWindow, CV_WINDOW_AUTOSIZE );
  createTrackbar( "Quality:", HarrisWindow, &HarrisQualityLevel, maxQualityLevel, HarrisFunction );  
  HarrisFunction( 0, 0 );
  
  waitKey(0);
  return(0);
}

/// 角点检测函数实现
void HarrisFunction( int, void* )
{
	/// 深度拷贝原图像用于绘制角点
	srcCopy = src.clone();

	if( HarrisQualityLevel < 1 )
		HarrisQualityLevel = 1;

	/// 角点满足条件则绘制
	for( int j = 0; j < srcGray.rows; j++ )
	{ 
		for( int i = 0; i < srcGray.cols; i++ )
		{
			if( resHarris.at<float>(j,i) > HarrisMinVal + ( HarrisMaxVal - HarrisMinVal )*HarrisQualityLevel/maxQualityLevel )
				circle( srcCopy, Point(i,j), 2, Scalar( 0,0,255 ), -1, 8, 0 );
		}
	}
	imshow( HarrisWindow, srcCopy );
	cout<<"Harris Quality Level: "<<HarrisQualityLevel<<endl;
}