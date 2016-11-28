#include <iostream>
#include "opencv2/opencv.hpp"

int main(int argc,char* argv[]){
	cv::Mat image = cv::imread(argv[1]);
	if(image.empty()){
		std::cout<<"no image!"<<std::endl;
	}
	cv::HOGDescriptor hog;
	hog.setSVMDetector(cv::HOGDescriptor::getDefaultPeopleDetector());
	std::vector<cv::Rect> regions;
	hog.detectMultiScale(image,regions,0,cv::Size(8,8),cv::Size(32,32),1.05,1);
	for(int i=0;i<regions.size();i++){
		cv::rectangle(image,regions[i],cv::Scalar(0,0,255),2);
	}
	cv::imshow("行人检测",image);
	cv::waitKey(0);
	return 0;
}
