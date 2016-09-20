/*
*
* 2. 对raccoon_hw.jpg图片RGB三个通道分别生成颜色直方图，上传提交三个通道的颜色直方图。
*
*/

#include "stdafx.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include<iostream>
//using namespace std;
//using namespace cv;

int main(int argc, char* argv[]) {

	// 分离的BGR通道
	cv::Mat channelsRGB[3];

	// 读入图片
	// cv::Mat image = cv::imread("D:\\tmp\\image\\raccoon_hw.jpg");
	cv::Mat image = cv::imread("raccoon_hw.jpg");
	// 分离通道
	cv::split(image, channelsRGB);

	// 获取直方图
	int histsize[] = { 256 }; //项的数量
	float range[] = { 0, 256 }; //像素值的范围
	const float *ranges[] = { range };
	int channels[] = { 0 }; //通道

	//直方图结果
	cv::Mat hist[3];
	// 直方图的图像
	cv::Mat hist_img[3];
	// 颜色
	cv::Scalar colors[3] = { cv::Scalar(0, 0, 255) ,cv::Scalar(0, 255, 0) ,cv::Scalar(255, 0, 0) };

	for (int i = 0; i < 3; i++) {
		// 不使用图像作为掩码
		calcHist(&channelsRGB[i], 1, channels, cv::Mat() , hist[i], 1, histsize, ranges);
		// 以图形方式显示直方图
		// 获取最大值和最小值
		double maxVal = 0;
		double minVal = 0;
		cv::minMaxLoc(hist[i], &minVal, &maxVal, 0, 0);
		// 显示直方图的图像
		hist_img[i] = cv::Mat(histsize[0], histsize[0], CV_8UC3, cv::Scalar(255, 255, 255));
		// 设置最高点为图高的90%
		int high_pt = static_cast<int>(0.9*histsize[0]);
		// 每个条目绘制一条垂直线
		for (int h = 0; h<histsize[0]; h++) {
			float binVal = hist[i].at<float>(h);
			int intensity = static_cast<int>(binVal*high_pt / maxVal);
			// 两点之间绘制一条线
			cv::line(hist_img[i], cv::Point(h, histsize[0]), cv::Point(h, histsize[0] - intensity), colors[i]);
		}
	}

	// 复制图像
	const int width = hist_img[0].cols;
	const int height = hist_img[0].rows;
	cv::Mat show_image(cv::Size(width, 3 * height), CV_8UC3);
	hist_img[0].copyTo(show_image(cv::Rect(0, 0, width, height)));
	hist_img[1].copyTo(show_image(cv::Rect(0, height, width, height)));
	hist_img[2].copyTo(show_image(cv::Rect(0, 2 * height, width, height)));

	cv::imshow("hist", show_image);
	cv::waitKey(0);
	return 0;
}
