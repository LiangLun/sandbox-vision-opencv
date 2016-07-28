#include "opencv2/opencv.hpp"
#include <ctime>

using namespace std;
using namespace cv;

double INITIAL_TIME;
double DURATION=5;

void updateMotionHistoryTemplate(Mat& prev_frame, Mat& frame, Mat& history)
{
    // 1-Calculate the silhouette of difference between the two frames
    absdiff(frame, prev_frame, prev_frame);

    // 2-Applying a threshold on the difference image
    double threshold_val = 100;
    threshold(prev_frame,prev_frame,threshold_val,255,THRESH_BINARY);

    // 3-Calculate the current time
    clock_t aux_time = clock();
    double current_time = (aux_time-INITIAL_TIME)/CLOCKS_PER_SEC;

    // 4-Performing the Update Motion History
    updateMotionHistory(prev_frame, history, current_time, DURATION);
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

    // Calculate the initial time
    INITIAL_TIME = clock()/CLOCKS_PER_SEC;

    // Create a Mat to save the Motion History
    Mat history(prev_frame.rows, prev_frame.cols, CV_32FC1);
    while(!finish)
    {
        // Read each frame, if possible
        if(!capture.read(frame)) return 1;

        // Convernto to gray image
        cvtColor(frame,frame,COLOR_BGR2GRAY);

        // Using Update Motion History Template
        updateMotionHistoryTemplate(prev_frame, frame, history);

        imshow("Video Camera",prev_frame);

        // Press Esc to finish
        if(waitKey(1)==27) finish=true;

        prev_frame = frame;
    }
    // Release the video camera
    capture.release();
    return 0;
}

int main()
{
    videoCamera();
}
