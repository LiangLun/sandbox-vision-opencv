#include "opencv2/opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

Mat src, dst, adaptDst;
int threshold_value, block_size, C;

void thresholding( int, void* )
{
  threshold( src, dst, threshold_value, 255, THRESH_BINARY );

  imshow( "Thresholding", dst );
}

void adaptThreshAndShow()
{
    adaptiveThreshold( src, adaptDst, 255, CV_ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, block_size, C);
    imshow( "Adaptive Thresholding", adaptDst );
}

void adaptiveThresholding1( int, void* )
{
  static int prev_block_size=block_size;
  if ((block_size%2)==0)    // make sure that block_size is odd
  {
      if (block_size>prev_block_size)  block_size++;
      if (block_size<prev_block_size) block_size--;
  }
  if (block_size<=1) block_size=3;  // check block_size min value

  adaptThreshAndShow();
}

void adaptiveThresholding2( int, void* )
{
    adaptThreshAndShow();
}

int main(int argc, char *argv[])
{
    // Read original image and clone it to contain results
    src = imread("left12.jpg", CV_LOAD_IMAGE_GRAYSCALE );
    dst=src.clone();
    adaptDst=src.clone();

    // Create 3 windows
    namedWindow("Source", WINDOW_AUTOSIZE);
    namedWindow("Thresholding", WINDOW_AUTOSIZE);
    namedWindow("Adaptive Thresholding", WINDOW_AUTOSIZE);
    imshow("Source", src);

    // Create trackbars
    threshold_value=127;
    block_size=7;
    C=10;
    createTrackbar( "threshold", "Thresholding", &threshold_value, 255, thresholding );
    createTrackbar( "block_size", "Adaptive Thresholding", &block_size, 25, adaptiveThresholding1 );
    createTrackbar( "C", "Adaptive Thresholding", &C, 255, adaptiveThresholding2 );

    // Perform operations a first time
    thresholding(threshold_value,0);
    adaptiveThresholding1(block_size, 0);
    adaptiveThresholding2(C, 0);

    // Position windows on screen
    moveWindow("Source", 0,0);
    moveWindow("Thresholding", src.cols,0);
    moveWindow("Adaptive Thresholding", 2*src.cols,0);

    cout << "Press any key to exit...\n";
    waitKey(); // Wait for key press
    return 0;
}

