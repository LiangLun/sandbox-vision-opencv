#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

int main( int,  char *argv[] )
{
  Mat src, image, hist;
  int histSize = 255;
  long int dim;


  // Read original image
  src = imread( "fruits.jpg");

  /// Convert to grayscale
  cvtColor( src, src, COLOR_BGR2GRAY );

  // Apply Histogram Equalization
  equalizeHist( src, image );

  // Display results
  namedWindow("Source image", 0 );
  namedWindow("Equalized Image", 0 );

  imshow( "Source image", src );
  imshow( "Equalized Image", image );

  // Calculate Histogram of the Equalized Image and display
  calcHist(&image, 1, 0, Mat(), hist, 1, &histSize, 0);
  dim=image.rows *image.cols;
  Mat histImage = Mat::ones(255, 255, CV_8U)*255;

  normalize(hist, hist, 0, histImage.rows, CV_MINMAX, CV_32F);

  histImage = Scalar::all(255);
  int binW = cvRound((double)histImage.cols/histSize);

  for( int i = 0; i < histSize; i++ )
      rectangle( histImage, Point(i*binW, histImage.rows),
                 Point((i+1)*binW, histImage.rows - cvRound(hist.at<float>(i))),
                 Scalar::all(0), -1, 8, 0 );
  namedWindow("Histogram Equalized Image", WINDOW_AUTOSIZE);
  imshow("Histogram Equalized Image", histImage);

  // Wait until user exits the program
  waitKey();
  return 0;
}


