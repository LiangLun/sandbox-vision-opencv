#include "opencv2/opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
    // Read original image and clone it
    Mat src = imread("stuff.jpg" );
    Mat tgt = src.clone();

    // Create source window
    namedWindow("Source", WINDOW_AUTOSIZE);
    imshow("Source", src);
    moveWindow("Source", 0,0);

    // GrabCut segmentation
    Rect rectangle(180,279,60,60);  // coin position
    Mat result;                     // segmentation result
    Mat bgModel,fgModel;            // used internally
    grabCut(src, result, rectangle, bgModel,fgModel, 1, GC_INIT_WITH_RECT);

    result=(result & GC_FGD);   // leave only obvious foreground

    // Translation operation
    Mat aff=Mat::eye(2,3,CV_32FC1);
    aff.at<float>(0,2)=50;
    warpAffine(tgt, src, aff, result.size());
    warpAffine(result, result, aff, result.size());
    src.copyTo(tgt, result);

    // Show target window
    imshow("Target", tgt);
    moveWindow("Target", src.cols, 0);

    cout << "Press any key to exit...\n";
    waitKey(); // Wait for key press
    return 0;
}

