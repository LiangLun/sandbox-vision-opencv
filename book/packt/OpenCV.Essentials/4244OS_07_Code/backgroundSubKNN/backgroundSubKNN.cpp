#include<opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int backGroundSubKNN()
{
    // 1-Set the parameters and initializations
    Mat frame;
    Mat background;
    Mat foreground;
    bool finish = false;
    int history = 500;
    double dist2Threshold = 400.0;
    bool detectShadows = false;
    vector< vector<Point> > contours;
    namedWindow("Frame");
    namedWindow("Background");
    VideoCapture capture(0);

    // Check if video camera is opened
    if(!capture.isOpened()) return 1;

    // 2-Create the background subtractor KNN
    Ptr <BackgroundSubtractorKNN> bgKNN = createBackgroundSubtractorKNN (history, dist2Threshold, detectShadows);

    while(!finish)
    {
        // 3-Read each frame, if possible
        if(!capture.read(frame)) return 1;

        // 4-Using apply and getBackgroundImage method to get
        // foreground and background from this frame
        bgKNN->apply(frame,foreground);
        bgKNN->getBackgroundImage(background);

        // 5-Reduce the foreground noise
        erode(foreground, foreground, Mat());
        dilate(foreground, foreground, Mat());

        // 6-Find the foreground contours
        findContours(foreground,contours,RETR_EXTERNAL,CHAIN_APPROX_NONE);
        drawContours(frame,contours,-1,Scalar(0,0,255),2);

        // 7-Show the results
        imshow("Frame", frame);
        imshow("Background", background);
        moveWindow("Frame", 0, 100);
        moveWindow("Background",800, 100);

        // Press Esc to finish
        if(waitKey(1) == 27) finish = true;
    }
    capture.release();
    return 0;
}

int main( )
{
    backGroundSubKNN();
}
