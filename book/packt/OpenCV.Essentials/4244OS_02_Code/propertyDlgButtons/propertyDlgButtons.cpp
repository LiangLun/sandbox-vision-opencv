#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>

using namespace std;
using namespace cv;

Mat image;
const char win[]="Flip image";

void on_flipV(int state, void *p)
{
    flip(image, image, 0);  // flip vertical
    imshow(win, image);
}

void on_flipH(int state, void *p)
{
    flip(image, image, 1);  // flip horizontal
    imshow(win, image);
}

void on_negative(int state, void *p)
{
    bitwise_not(image, image);  // invert all channels
    imshow(win, image);
}

int main(int argc, char *argv[])
{
    if (argc != 2) {//Check args.
        cout << "Usage: <cmd> <file_in>\n";
        return -1;
    }
    image = imread(argv[1]);
    if (image.empty()) {
        cout << "Error! Input image cannot be read...\n";
        return -1;
    }

    namedWindow(win);
    imshow(win, image);
    displayOverlay(win, argv[1], 0);
    createButton("Flip Vertical", on_flipV, NULL, CV_PUSH_BUTTON);
    createButton("Flip Horizontal", on_flipH, NULL, CV_PUSH_BUTTON);
    int v=0;
    createTrackbar("trackbar1", "", &v, 255);
    createButton("Negative", on_negative, NULL, CV_CHECKBOX);

    cout << "Press any key to exit...\n";
    waitKey();
    return 0;
}
