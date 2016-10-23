/*
《OpenCV第1课》书面作业
按照PPT上的要求，在Windows或者Linux上安装好OpenCV软件，并且可以通过测试代码做使用。没安装成功的话，后续的课程就比较麻烦了哦~
*/
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
int main(int argc, char* argv[]){
  const std::string window_name = "lena";
  const std::string input_pic = "lena.jpg";
  cv::Mat test_pic = cv::imread(input_pic);
  if(test_pic.empty()){
    std::cout<<"no input image"<<std::endl;
    return -1;
  }
  cv::namedWindow(window_name);
  cv::imshow(window_name,test_pic);
  cv::waitKey(0);
}
