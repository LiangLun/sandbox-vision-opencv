// houghCircle.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <highgui.h>
#include "ExtendHoughCircle.h"

int _tmain(int argc, _TCHAR* argv[])
{
	std::string path="123.jpg";
	cv::Mat testMat=cv::imread(path);
	cv::vector<circle_found> found;
	ExtendCV::FindCircles(testMat,found,1,1,183,332,10,68,73,0.5);

	cv::circle(testMat,cv::Point(found[0].circle[0],found[0].circle[1]),found[0].circle[2],cv::Scalar(0,0,255),2);
	cv::imshow("hehe",testMat);

	cv::waitKey();

	return 0;
}

