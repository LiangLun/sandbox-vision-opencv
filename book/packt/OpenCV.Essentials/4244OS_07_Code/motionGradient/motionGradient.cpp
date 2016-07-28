#include "opencv2/opencv.hpp"
#include <ctime>

using namespace std;
using namespace cv;

double INITIAL_TIME;
double DURATION=5;

void motionGradientMethod(Mat& history, Mat& orientations)
{
    // 1-Set the parameters
    double max_gradient = 3.0;
    double min_gradient = 1.0;
    // The default 3x3 Sobel filter
    int apertura_size = 3;
    // Distance to show the results
    int dist = 20;
    Mat mask = Mat::ones(history.rows, history.cols, CV_8UC1);

    // 2-Calcule motion gradients
    calcMotionGradient(history, mask, orientations, max_gradient, min_gradient, apertura_size);

    // 3-Show the results
    Mat result = Mat::zeros(orientations.rows, orientations.cols, CV_32FC1);
    for (int i=0;i<orientations.rows; i++)
    {
        for (int j=0;j<orientations.cols; j++)
        {
            double angle = 360-orientations.at<float>(i,j);
            if (angle!=360)
            {
                Point point_a(j,i);
                Point point_b(round(j+cos(angle)*dist), round(i+sin(angle)*dist));
                line(result, point_a, point_b, Scalar(255,0,0), 1);
            }
        }
    }
    imshow("Result", result);
}

void updateMotionHistoryTemplate(Mat& prev_frame, Mat& frame, Mat& history)
{
    // 1-Calculate the silhouette of difference between the two frames
    absdiff(frame, prev_frame, prev_frame);

    // 2-Applying a threshold on the difference image
    double threshold_value = 100;
    threshold(prev_frame,prev_frame,threshold_value,255,THRESH_BINARY);

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
    cvtColor(prev_frame,prev_frame,COLOR_RGB2GRAY);

    // Calculate the initial time
    INITIAL_TIME = clock()/CLOCKS_PER_SEC;

    // Create a Mat to save the Motion History
    Mat history(prev_frame.rows, prev_frame.cols, CV_32FC1);
    while(!finish)
    {
        // Read each frame, if possible
        if(!capture.read(frame)) return 1;

        // Convernto to gray image
        cvtColor(frame,frame,COLOR_RGB2GRAY);

        // Using Update Motion History Template
        updateMotionHistoryTemplate(prev_frame, frame, history);

        // Calculate motion gradients
        Mat orientations = Mat::ones(history.rows, history.cols, CV_32FC1);
        motionGradientMethod(history, orientations);

        imshow("Video Camera", prev_frame);

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
