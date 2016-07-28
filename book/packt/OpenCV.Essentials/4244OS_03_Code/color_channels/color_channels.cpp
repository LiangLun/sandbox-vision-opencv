#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

int main( )
{
       Mat image, HSV, Luv, Lab, YCrCb, XYZ;

       // Read image
       image = imread("HappyFish.jpg", CV_LOAD_IMAGE_COLOR);

       // Convert RGB image to different color spaces
       cvtColor(image, HSV, CV_RGB2HSV);
       cvtColor(image, Luv, CV_RGB2Luv);
       cvtColor(image, Lab, CV_RGB2Lab);
       cvtColor(image, YCrCb, CV_RGB2YCrCb);
       cvtColor(image, XYZ, CV_RGB2XYZ);

      // Create windows to display the original image and the results
       namedWindow( "Source Image", 0 );
       imshow( "Source Image", image );

       namedWindow( "Result HSV Image", 0 );
       imshow( "Result HSV Image",  HSV );

       namedWindow( "Result Luv Image", 0 );
       imshow( "Result Luv Image", Luv );

       namedWindow( "Result Lab Image", 0 );
       imshow( "Result Lab Image", Lab);

       namedWindow( "Result YCrCb Image", 0 );
       imshow( "Result YCrCb Image", YCrCb );

       namedWindow( "Result XYZ Image", 0 );
       imshow( "Result XYZ Image", XYZ );

       waitKey(); // Wait for key press
       return 0;  // En the program
}

