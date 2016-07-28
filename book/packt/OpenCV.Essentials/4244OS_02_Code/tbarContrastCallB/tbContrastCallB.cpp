#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>

using namespace std;
using namespace cv;

#define IN_WIN "Orig. image"
#define OUT_WIN "Image converted...(no saved)"
Mat in_img, out_img;

// CallBack function for contrast TrackBar
void updateContrast(int TBvalContrast, void *userData=0) {

    in_img.convertTo(out_img, -1, TBvalContrast/50.0);
    imshow(OUT_WIN, out_img);
    return;
}

int main(int argc, char* argv[]) {

    int TBvalContrast=50; // Value of the TrackBar

    if (argc != 2) {
        cout << "Usage: <cmd> <input image_file>" << endl;
        return -1;
    }
    in_img = imread(argv[1]); // Open and read the image
    if (in_img.empty()) {
        cout << "Error!!! Image cannot be loaded..." << endl;
        return -1;
    }
    in_img.copyTo(out_img); // Copy orig. image to final img
    namedWindow(IN_WIN); // Creates window for orig. image
    moveWindow(IN_WIN, 0, 0); // Move window to pos. (0, 0)
    imshow(IN_WIN, in_img); // Shows original image
    namedWindow(OUT_WIN); // Creates window for converted image
    createTrackbar("Contrast", OUT_WIN, &TBvalContrast, 100,
                   updateContrast);
    imshow(OUT_WIN, out_img); // Shows converted image
    cout << "Press a key to exit..." << endl;
    waitKey();
    return 0;
}
