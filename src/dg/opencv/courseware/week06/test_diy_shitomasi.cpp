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
Mat dstShiTomasi;	//dstShiTomasi用于存储角点检测的结果
/// 各类阈值
int ShiTomasiQualityLevel = 50;
int maxQualityLevel = 100;
double ShiTomasiMinVal;
double ShiTomasiMaxVal;
char* ShiTomasiWindow = "My Shi-Tomasi corner detector";

/// 角点检测函数声明
void ShiTomasiFunction( int, void* );

int main( int argc, char** argv )
{
	/// 载入图像并灰度化
	src = imread("./house_small.jpg", 1 );
	cvtColor( src, srcGray, CV_BGR2GRAY );

	/// 设置参数
	int blockSize = 3;
	int apertureSize = 3;

	/// 使用cornerMinEigenVal()函数检测角点
	dstShiTomasi = Mat::zeros( srcGray.size(), CV_32FC1 );  
	cornerMinEigenVal( srcGray, dstShiTomasi, blockSize, apertureSize, BORDER_DEFAULT );
	minMaxLoc( dstShiTomasi, &ShiTomasiMinVal, &ShiTomasiMaxVal, 0, 0, Mat() );

	/// 创建窗口和滑动条
	namedWindow( ShiTomasiWindow, CV_WINDOW_AUTOSIZE );   
	createTrackbar( " Quality:", ShiTomasiWindow, &ShiTomasiQualityLevel, maxQualityLevel, ShiTomasiFunction );  
	ShiTomasiFunction( 0, 0 );

	waitKey(0);
	return(0);
}

/// 角点检测函数实现
void ShiTomasiFunction( int, void* )
{
	/// 深度拷贝原图像用于绘制角点
	srcCopy = src.clone();

	if( ShiTomasiQualityLevel < 1 )
		ShiTomasiQualityLevel = 1;

	/// 角点满足条件则绘制
	for( int j = 0; j < srcGray.rows; j++ )
	{ 
		for( int i = 0; i < srcGray.cols; i++ )
		{
			if( dstShiTomasi.at<float>(j,i) > ShiTomasiMinVal + ( ShiTomasiMaxVal - ShiTomasiMinVal )*ShiTomasiQualityLevel/maxQualityLevel )
				circle( srcCopy, Point(i,j), 2, Scalar( 255,0,0 ), -1, 8, 0 );
		}
	}
	imshow( ShiTomasiWindow, srcCopy );
	cout<<"Shi-Tomasi Quality Level: "<<ShiTomasiQualityLevel<<endl;
}