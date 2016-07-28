#include "opencv2/opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

vector<Vec4i> lines;
vector<float> widths;
Mat input_image, output;

inline float line_length(const Point &a, const Point &b)
{
    return (sqrt((b.x-a.x)*(b.x-a.x) + (b.y-a.y)*(b.y-a.y)));
}

void MyDrawSegments(Mat &image, const vector<Vec4i> &lines, const vector<float> &widths,
                    const Scalar& color, const float length_threshold)
{
    Mat gray;
    if (image.channels() == 1)
    {
        gray = image;
    }
    else if (image.channels() == 3)
    {
        cvtColor(image, gray, COLOR_BGR2GRAY);
    }

    // Create a 3 channel image in order to draw colored lines
    std::vector<Mat> planes;
    planes.push_back(gray);
    planes.push_back(gray);
    planes.push_back(gray);

    merge(planes, image);

    // Draw segments if length exceeds threshold given
    for(int i = 0; i < lines.size(); ++i)
    {
        const Vec4i& v = lines[i];
        Point a(v[0], v[1]);
        Point b(v[2], v[3]);
        if (line_length(a,b) > length_threshold) line(image, a, b, color, widths[i]);
    }
}


void thresholding(int threshold, void*)
{
    input_image.copyTo(output);
    MyDrawSegments(output, lines, widths, Scalar(0, 255, 0), threshold);
    imshow("Detected lines", output);
}

int main(int argc, char** argv)
{
    input_image = imread("building.jpg", IMREAD_GRAYSCALE);

    // Create an LSD detector object
    Ptr<LineSegmentDetector> ls = createLineSegmentDetector();

    // Detect the lines
    ls->detect(input_image, lines, widths);

    // Create window to show found lines
    output=input_image.clone();
    namedWindow("Detected lines", WINDOW_AUTOSIZE);

    // Create trackbar for line length threshold
    int threshold_value=50;
    createTrackbar( "Line length threshold", "Detected lines", &threshold_value, 1000, thresholding );
    thresholding(threshold_value, 0);

    waitKey();
    return 0;
}
