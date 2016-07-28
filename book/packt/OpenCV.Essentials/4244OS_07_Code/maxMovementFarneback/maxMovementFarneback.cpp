#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

const int MAX_MOVEMENT = 100;
const int MIN_MOVEMENT = 10;

void maxMovementFarneback(Mat& prev_frame, Mat& frame)
{
    // 1-Set the Parameters
    Mat optical_flow = Mat(prev_frame.size(), COLOR_BGR2GRAY);
    double pyr_scale = 0.5;
    int levels = 3;
    int win_size = 5;
    int iterations = 5;
    int poly_n = 5;
    double poly_sigma = 1.1;
    int flags = 0;

    // 2-Farneback method for optical flow
    calcOpticalFlowFarneback(prev_frame, frame, optical_flow, pyr_scale, levels, win_size, iterations, poly_n, poly_sigma, flags);

    // 3-Show the movements
    int max_move = 0;
    for (int i = 1; i < optical_flow.rows ; i++)
    {
        for (int j = 1; j < optical_flow.cols ; j++)
        {
            Point2f &p = optical_flow.at<Point2f>(i, j);
            Point pA = Point(round(i + p.x),round(j + p.y));
            Point pB = Point(i, j);
            int move = sqrt(p.x*p.x + p.y*p.y);
            if( move > MIN_MOVEMENT )
            {
                line(prev_frame, pA, pB, Scalar(255,0,0),2);
                if ( move > max_move )
                    max_move = move;
            }
        }
    }
    if(max_move > MAX_MOVEMENT)
    {
        putText(prev_frame,"INTRUDER",Point(100,100),FONT_ITALIC,3,Scalar(255,0,0),5);
        imshow("Video Camera", prev_frame);
        cout << "Press a key to continue..." << endl;
        waitKey();
    }
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

        // Detect Maximum Movement with Lucas-Kanade Method
        maxMovementFarneback(prev_frame, frame);

        imshow("Video Camera",prev_frame);

        // Press Esc to finish
        if(waitKey(1)==27) finish=true;

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
