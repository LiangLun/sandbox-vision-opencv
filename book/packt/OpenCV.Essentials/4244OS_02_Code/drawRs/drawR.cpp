#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>

using namespace std;
using namespace cv;

#define IN_WIN "Orig. image"
Mat orig_img, tmp_img;

// CallBack Function for mouse events
void cbMouse(int event, int x, int y, int flags, void* userdata) {

    static Point p1, p2; // Static vars hold values between calls
    static bool p2set = false;

    if (event == EVENT_LBUTTONDOWN) { // Left mouse button pressed
        p1 = Point(x, y); // Set orig. point
        p2set = false;
    } else if (event == EVENT_MOUSEMOVE && flags == EVENT_FLAG_LBUTTON) {
        if (x > orig_img.size().width) // Check out bounds
            x = orig_img.size().width;
        else if (x < 0)
            x = 0;
        if (y > orig_img.size().height) // Check out bounds
            y = orig_img.size().height;
        else if (y < 0)
            y = 0;
        p2 = Point(x, y); // Set final point
        p2set = true;
        orig_img.copyTo(tmp_img); // Copy orig. to temp. image
        rectangle(tmp_img, p1, p2, Scalar(0, 0 ,255));
        imshow(IN_WIN, tmp_img); // Draw temporal image with rect.
    } else if (event == EVENT_LBUTTONUP && p2set) {
        Mat submat = orig_img(Rect(p1, p2)); // Set subarray on orig. image
        submat.convertTo(submat, -1, 2.0); // Double the contrast of subarray
        rectangle(orig_img, p1, p2, Scalar(0, 0, 255));
        imshow(IN_WIN, orig_img); // Show image on window
    }
    return;
}

int main(int argc, char* argv[]) {

    if (argc != 2) {
        cout << "Usage: <cmd> <input image_file>" << endl;
        return -1;
    }
    orig_img = imread(argv[1]); // Open and read the image
    if (orig_img.empty()) {
        cout << "Error!!! Image cannot be loaded..." << endl;
        return -1;
    }
    namedWindow(IN_WIN); // Creates window for orig. image
    setMouseCallback(IN_WIN, cbMouse, NULL);
    imshow(IN_WIN, orig_img); // Shows original image
    cout << "Press any key to exit..." << endl;
    waitKey();
    return 0;
}
