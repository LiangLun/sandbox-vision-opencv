#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

void trackingMeanShift(Mat& img, Rect search_window)
{
    // 1-Criteria to MeanShift function
    TermCriteria criteria(TermCriteria::COUNT | TermCriteria::EPS, 10, 1);

    // 2-Tracking using MeanShift
    meanShift(img, search_window, criteria);

    // 3-Show the result
    rectangle(img, search_window, Scalar(0,255,0), 3);
}

int videoCamera()
{
    // Open the video camera
    VideoCapture capture(0);

    // Check if video camera is opened
    if(!capture.isOpened()) return 1;

    bool finish = false;
    Mat frame;
    Mat prev_frame;
    namedWindow("Video Camera");

    if(!capture.read(prev_frame)) return 1;

    // Convernto to gray image
    cvtColor(prev_frame,prev_frame,COLOR_BGR2GRAY);

    while(!finish)
    {
        // Read each frame, if possible
        if(!capture.read(frame)) return 1;

        // Convernto to gray image
        cvtColor(frame,frame,COLOR_BGR2GRAY);

        // Tracking using MeanShift with an initial search window
        Rect search_window(200,150,100,100);
        trackingMeanShift(prev_frame, search_window);

        imshow("Video Camera",prev_frame);

        // Press Esc to finish
        if(waitKey(1)==27) finish = true;

        prev_frame = frame;
    }
    // Release the video camera
    capture.release();
    return 0;
}

int main( )
{
    videoCamera();
}
