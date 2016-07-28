#include "opencv2/highgui/highgui.hpp"

using namespace cv;
using namespace std;


int main()
{
  double alpha = 0.5, beta, input;

    Mat src1, src2, dst;

    // Read image (same size, same type )
    src1 = imread("baboon.jpg");
    src2 = imread("lena.jpg");

    // Create windows
    namedWindow("Final Linear Blend", CV_WINDOW_AUTOSIZE );

    // Perform a loop with 101 iteration for linear blending
    for(int k = 0; k <= 100; ++k )
    {
        alpha = (double)k/100;
        beta  = 1 - alpha;

        addWeighted( src2, alpha, src1, beta, 0.0, dst );

        imshow( "Final Linear Blend", dst );
        cvWaitKey(50);
    }

    namedWindow("Original Image 1", CV_WINDOW_AUTOSIZE );
    namedWindow("Original Image 2", CV_WINDOW_AUTOSIZE );
    imshow( "Original Image 1", src1 );
    imshow( "Original Image 2", src2 );

    cvWaitKey(); // Wait for key press
    return 0;   // End
}
