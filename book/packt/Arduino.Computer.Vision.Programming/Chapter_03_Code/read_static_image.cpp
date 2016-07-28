#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{
    if( argc != 2)
    {
    cout <<" Usage: read_static_image   lena.jpg" << endl;
      return -1;
    }

    Mat image;
    // Read the image file name
    image = imread(argv[1], CV_LOAD_IMAGE_COLOR);   

  // Check for invalid input
    if( image.data == NULL )                              
    {
        cout <<  "Couldn’t open the image" << endl ;
        return -1;
    }
    // Create a window for display.
    namedWindow( "Read Static Image", WINDOW_AUTOSIZE );
    //Display the image
    imshow( "Read Static Image", image );                   
    // Wait for a keystroke in the window
    waitKey(0);                                          
    return 0;
}
