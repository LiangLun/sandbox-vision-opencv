#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
    Mat frame; // Container for each frame

    // Check the right number of command line arguments
    if (argc != 2) {
        cout << "USAGE: <cmd> <file_in>\n";
        cout << "Note: Video file must have a proper format (avi container)\n";
        return -1;
    }

    // Creates a VideoCapture object and open original video file
    VideoCapture vid(argv[1]);
    if (!vid.isOpened()) { // Check if video file was opened
        cout << "ERROR: Video file not opened...\n";
        return -1;
    }

    // Get fps from video input
    int fps = (int)vid.get(CV_CAP_PROP_FPS); // Gets FPS from video
    cout << "FPS=" << fps << endl;

    // Create a window for displaying the video
    namedWindow(argv[1]); // Creates a window
    while (1) {
        if (!vid.read(frame)) { // Check end of video file
            cout << "Video file ends.\n";
            break;
        }
        // Show the frame on window
        imshow(argv[1], frame); // Show current frame on window
        // Wait for read next frame or key pressed
        if (waitKey(1000/fps) >= 0)
            break;
    }
    return 0;
}

