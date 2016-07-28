#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{
  if( argc != 2)
      {
    cout <<" Usage: take_snapshot snapshot.jpg" << endl;
        return -1;
      }
  //create the capture object
  VideoCapture cap(CV_CAP_ANY);

  // Get the frame
  Mat frame;
  cap >> frame;
  //Check if frame is empty or not
  if(frame.empty())
  {
    cerr << "Could not get frame from camera" << endl;
    return -1;
  }
  // Save the frame into a file
  imwrite(argv[1], frame); 

  return 0;
}
