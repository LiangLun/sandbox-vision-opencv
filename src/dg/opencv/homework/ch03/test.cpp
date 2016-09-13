/*
 *
 * 1. 在“作业素材”中，有一张horse_hw.jpg的图片，对图片中做目标分割，生成分割后的图片（黑白图）。
 * 2. 在horse_hw.jpg图像内画一个换色的矩形框，在矩形框内用白色写上自己的名字。
 * 3. 把1和2的两张图片上下摆放到图一个输出显示框内。
 * 提交方式：提交最终的效果图片和代码。
 * TIP：
 * 1和3 在实例2中都有体现如何实现, 2主要使用函数rectangle(),putText()。
 *
 */

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

int main(int argc,char* argv[]){

	//--1.读入图片
	cv::Mat image = cv::imread("horse_hw.jpg");

	//--2.转换灰度图
	cv::Mat gray;
	cv::cvtColor(image,gray,CV_RGB2GRAY);
	
	//--3.二值化灰度图
	cv::Mat dst;
	cv::threshold(gray,dst,60,255,cv::THRESH_BINARY_INV);
	//平滑处理(平滑/高斯平滑两种)
	//cv::blur(dst,dst,cv::Size(3,3));
	//cv::GaussianBlur(dst,dst,cv::Size(3,3),0,0); //高斯平滑
	
	//形态学操作（此处效果不好，只用来展示加强理解）
	//cv::erode(dst,dst,cv::Mat(),cv::Point(-1,-1),1);
	//cv::dilate(dst,dst,cv::Mat(),cv::Point(-1,-1),1);
	
	//--4.显示结果(原图和结果图显示在一起)
	const int width  = image.cols;
	const int height = image.rows;
	cv::Mat show_image(cv::Size(width,2*height),CV_8UC3);
	
	//将dst转换为3通道，使得show_image和dst通道数一致，或者使用convertTo()函数做操作
	cv::cvtColor(dst,dst,CV_GRAY2RGB);
	// 拷贝到显示图片指定位置（上方）
	dst.copyTo(show_image(cv::Rect(0,0,width,height)));
	
	
	// 矩形框
	rectangle(image,a,Point(255,255),Scalar(255,0,0));
	// 附加文字
	putText( image, "dataguru", Point( image.rows/2,image.cols/4),CV_FONT_HERSHEY_COMPLEX, 1, Scalar(255, 0, 0) );
	// 拷贝到显示图片指定位置（下方）
	image.copyTo(show_image(cv::Rect(0,height,width,height)));
	
	//显示结果
	cv::imshow("show",show_image);
	cv::waitKey(0);
}
