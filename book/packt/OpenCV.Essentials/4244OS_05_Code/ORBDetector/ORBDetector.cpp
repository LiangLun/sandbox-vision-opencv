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

    //ORB detector with FAST (detector1) and HARRIS (detector2) score to rank the features
    OrbFeatureDetector detector1(300, 1.1f, 2, 31,0, 2, ORB::FAST_SCORE, 31);
    OrbFeatureDetector detector2(300, 1.1f, 2, 31,0, 2, ORB::HARRIS_SCORE, 31);

    //Compute keypoints in in_img with detector1 and detector2
    detector1.detect(in_img, keypoints1);
    detector2.detect(in_img, keypoints2);

    Mat out_img1, out_img2;
    //Draw keypoints1 and keypoints2
    drawKeypoints(in_img,keypoints1,out_img1,Scalar::all(-1),DrawMatchesFlags::DEFAULT);
    drawKeypoints(in_img,keypoints2,out_img2,Scalar::all(-1),DrawMatchesFlags::DEFAULT);

    //Show the 2 final images
    imshow( "out_img1", out_img1 );//Show result of detector1
    imshow( "out_img2", out_img2 );//Show result of detector2

    waitKey(0);
    return 0;
}

