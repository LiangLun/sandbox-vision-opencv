#include <opencv2/core/utility.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/core.hpp"
#include <iostream>

using namespace std;
using namespace cv;


void Watershed(const Mat &src)
{
    Mat dst=src.clone();

    // Flood fill outer part of the image
    Point seed(0,0); // top-left corner
    int loDiff=20;
    int upDiff=20;
    int flags=4 + FLOODFILL_FIXED_RANGE + FLOODFILL_MASK_ONLY + (255<<8);
    Mat mask(src.size(), CV_8UC1);
    mask.setTo(0);
    copyMakeBorder(mask, mask, 1, 1, 1, 1, cv::BORDER_REPLICATE);
    Scalar newVal;
    Rect ccomp;
    floodFill(dst, mask, seed, newVal, &ccomp,
         Scalar(loDiff, loDiff, loDiff), Scalar(upDiff, upDiff, upDiff), flags);

    // Flood fill inner part of the image
    seed.x=(float)src.cols/2;   // image center x
    seed.y=(float)src.rows/2;   // image center y
    Mat mask1=mask.clone();
    mask1.setTo(0);
    floodFill(dst, mask1, seed, newVal, &ccomp,
          Scalar(loDiff, loDiff, loDiff), Scalar(upDiff, upDiff, upDiff), flags);

    // Form image with the two seed regions
    Mat Mask = mask.clone();
    mask=mask/2;
    Mask = mask | mask1;
    imshow("Seed regions", Mask);
    moveWindow("Seed regions", src.cols, 0);

    // Perform watershed
    Mat labelImage(src.size(), CV_32SC1);
    labelImage=Mask(Rect(1,1, src.cols, src.rows));
    labelImage.convertTo(labelImage, CV_32SC1);
    watershed(src, labelImage);
    labelImage.convertTo(labelImage, CV_8U);
    imshow("Watershed", labelImage);
    moveWindow("Watershed", 2*src.cols, 0);
}


int main(int argc, char *argv[])
{
    // Read original image and clone it to contain results
    Mat src = imread("hand_sample2.jpg", IMREAD_COLOR );

    // Create 3 windows
    namedWindow("Source", WINDOW_AUTOSIZE);
    imshow("Source", src);

    Watershed(src);

    // Position windows on screen
    moveWindow("Source", 0,0);

    cout << "Press any key to exit...\n";
    waitKey(); // Wait for key press
    return 0;
}

