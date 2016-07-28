#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
    //Load image and convert to gray scale
    Mat in_img = imread("book.png");
    cvtColor( in_img, in_img, COLOR_BGR2GRAY);

    //Create a keypoint vectors
    vector<KeyPoint> keypoints1,keypoints2;

    //SURF detector1 and detector2 with 2 and 5 gaussian pyramid octaves respectively
    SurfFeatureDetector detector1(3500, 2, 2, false, false);
    SurfFeatureDetector detector2(3500, 5, 2, false, false);

    //Compute keypoints in in_img with detector1 and detector2
    detector1.detect(in_img, keypoints1);
    detector2.detect(in_img, keypoints2);

    Mat out_img1, out_img2;
    //Draw keypoints1 and keypoints2
    drawKeypoints(in_img,keypoints1,out_img1,Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
    drawKeypoints(in_img,keypoints2,out_img2,Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

    //Show the 2 final images
    imshow( "out_img1", out_img1 ); //Show result of detector1
    imshow( "out_img2", out_img2 ); //Show result of detector2
    waitKey(0);
    return 0;
}

