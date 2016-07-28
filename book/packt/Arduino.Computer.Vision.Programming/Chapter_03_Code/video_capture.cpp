#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>

using namespace cv;
using namespace std;

void ProcessFrame(Mat& newFrame)
{
  cvtColor(newFrame, newFrame, CV_BGR2GRAY);
}

int main( int argc, char** argv )
{
  //create the capture object
  VideoCapture cap(CV_CAP_ANY);

  while(1)
  {
    // Get the frame
    Mat frame;
    cap >> frame;
    //Check if frame is empty or not
    if(frame.empty())
    {
      cerr << " could not get frame." << endl;
      return -1;
    }
    ProcessFrame(frame);
    imshow("Live vide stream",frame);
    if(waitKey(30) >= 0) break;
  }
  return 0;
}
