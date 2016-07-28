#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d/features2d.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{

    //Load original image and convert to gray scale
    Mat in_img = imread("book.png");
    cvtColor( in_img, in_img, COLOR_BGR2GRAY);

    //Create a keypoint vectors
    vector<KeyPoint> keypoints1,keypoints2;

    //Create KAZE and AKAZE detectors
    KAZE detector1(true,true);
    AKAZE detector2(cv::AKAZE::DESCRIPTOR_KAZE_UPRIGHT,0,3);

    //Compute keypoints in in_img with detector1 and detector2
    detector1.detect(in_img, keypoints1);
    detector2.detect(in_img, keypoints2,cv::Mat());

    Mat out_img1, out_img2;
    //Draw keypoints1 and keypoints2
    drawKeypoints(in_img,keypoints1,out_img1,Scalar::all(-1),4);
    drawKeypoints(in_img,keypoints2,out_img2,Scalar::all(-1),4);

    //Show result of detector1 and detector2
    imshow( "out_img1", out_img1 );
    imshow( "out_img2", out_img2 );
    waitKey(0);
    return 0;
}


