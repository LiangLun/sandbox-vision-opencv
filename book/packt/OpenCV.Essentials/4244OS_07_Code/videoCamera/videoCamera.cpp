#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

int videoCamera()
{
    // 1-Open the video camera
    VideoCapture capture(0);

    // Check if video camera is opened
    if(!capture.isOpened()) return 1;

    bool finish = false;
    Mat frame;
    Mat prev_frame;
    namedWindow("Video Camera");

    if (!capture.read(prev_frame)) return 1;

    // Convert to gray image
    cvtColor(prev_frame,prev_frame,COLOR_BGR2GRAY);

    while(!finish)
    {
        // 2-Read each frame, if possible
        if(!capture.read(frame)) return 1;

        // Convert to gray image
        cvtColor(frame ,frame, COLOR_BGR2GRAY);

        imshow("Video Camera", prev_frame);

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
