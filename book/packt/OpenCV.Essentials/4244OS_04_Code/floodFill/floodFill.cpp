#include "opencv2/opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

Mat image, image1, image_orig;
int loDiff = 20, upDiff = 30;
int loCanny=10, upCanny=150;

void onMouse( int event, int x, int y, int, void* )
{
    if( event != CV_EVENT_LBUTTONDOWN ) return;

    Point seed = Point(x,y);
    int flags = 4 + CV_FLOODFILL_FIXED_RANGE;
    int b = (unsigned)theRNG() & 255;
    int g = (unsigned)theRNG() & 255;
    int r = (unsigned)theRNG() & 255;
    Rect ccomp;

    Scalar newVal = Scalar(b, g, r);
    Mat dst = image;

    // flood fill
    floodFill(dst, seed, newVal, &ccomp, Scalar(loDiff, loDiff, loDiff),
                    Scalar(upDiff, upDiff, upDiff), flags);
    imshow("image", dst);

    // Using Canny edges as mask
    Mat mask;
    Canny(image_orig, mask, loCanny, upCanny);
    imshow("Canny edges", mask);
    copyMakeBorder(mask, mask, 1, 1, 1, 1, cv::BORDER_REPLICATE);
    Mat dst1 = image1;
    floodFill(dst1, mask, seed, newVal, &ccomp, Scalar(loDiff, loDiff, loDiff),
                    Scalar(upDiff, upDiff, upDiff), flags);
    imshow("FF with Canny", dst1);

    moveWindow("Canny edges", image.cols,0);
    moveWindow("FF with Canny", 2*image.cols,0);
}

int main(int argc, char *argv[])
{
    // Read original image and clone it to contain results
    image = imread("lena.jpg", CV_LOAD_IMAGE_COLOR );
    image_orig=image.clone();
    image1=image.clone();

    namedWindow( "image", WINDOW_AUTOSIZE );

    imshow("image", image);
    createTrackbar( "lo_diff", "image", &loDiff, 255, 0 );
    createTrackbar( "up_diff", "image", &upDiff, 255, 0 );
    createTrackbar( "lo_Canny", "image", &loCanny, 255, 0 );
    createTrackbar( "up_Canny", "image", &upCanny, 255, 0 );
    setMouseCallback( "image", onMouse, 0 );

    moveWindow("image", 0,0);

    cout << "Press any key to exit...\n";
    waitKey(); // Wait for key press
    return 0;
}

