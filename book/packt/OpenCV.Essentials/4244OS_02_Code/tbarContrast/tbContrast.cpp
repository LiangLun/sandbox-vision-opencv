#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char* argv[]) {

    const char in_win[]="Orig. image";
    const char out_win[]="Image converted...(no saved)";
    int TBvalContrast=50; // Value of the TrackBar
    Mat out_img;

    if (argc != 2) {
        cout << "Usage: <cmd> <input image_file>" << endl;
        return -1;
    }
    Mat in_img = imread(argv[1]); // Open and read the image
    if (in_img.empty()) {
        cout << "Error!!! Image cannot be loaded..." << endl;
        return -1;
    }
    namedWindow(in_win); // Creates window for orig. image
    moveWindow(in_win, 0, 0); // Move window to pos. (0, 0)
    imshow(in_win, in_img); // Shows original image
    namedWindow(out_win);
    createTrackbar("Contrast", out_win, &TBvalContrast, 100);
    cout << "Press Esc key to exit..." << endl;
    while (true) {
        in_img.convertTo(out_img, -1, TBvalContrast / 50.0);
        imshow(out_win, out_img);
        if (waitKey(50) == 27) // If Esc key pressed breaks
            break;
    }
    return 0;
}
