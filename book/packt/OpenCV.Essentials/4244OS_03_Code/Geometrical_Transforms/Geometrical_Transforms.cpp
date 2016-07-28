#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

Point2f center(0,0);

Point2f computeIntersect(Vec4i a,Vec4i b)
{
    int x1 = a[0], y1 = a[1], x2 = a[2], y2 = a[3], x3 = b[0], y3 = b[1], x4 = b[2], y4 = b[3];

    if (float d = ((float)(x1 - x2) * (y3 - y4)) - ((y1 - y2) * (x3 - x4)))
    {
        Point2f pt;
        pt.x = ((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4)) / d;
        pt.y = ((x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4)) / d;
        return pt;
    }
    else
        return Point2f(-1, -1);
}

void sortCorners(vector<Point2f>& corners, Point2f center)
{
    vector<Point2f> top, bot;

    for (int i = 0; i < corners.size(); i++)
    {
        if (corners[i].y < center.y)
            top.push_back(corners[i]);
        else
            bot.push_back(corners[i]);
    }

    Point2f tl = top[0].x > top[1].x ? top[1] : top[0];
    Point2f tr = top[0].x > top[1].x ? top[0] : top[1];
    Point2f bl = bot[0].x > bot[1].x ? bot[1] : bot[0];
    Point2f br = bot[0].x > bot[1].x ? bot[0] : bot[1];

    corners.clear();
    corners.push_back(tl);
    corners.push_back(tr);
    corners.push_back(br);
    corners.push_back(bl);
}

int main()
{
    Mat src = imread("img.jpg");
    if (src.empty())
        return -1;

    Mat dst = src.clone();

    Mat bw;
    cvtColor(src, bw, CV_BGR2GRAY);

    Canny(bw, bw, 100, 100, 3);
    vector<Vec4i> lines;
    HoughLinesP(bw, lines, 1, CV_PI/180, 70, 30, 10);

    vector<Point2f> corners;
    for (int i = 0; i < lines.size(); i++)
    {
        for (int j = i+1; j < lines.size(); j++)
        {
            Point2f pt = computeIntersect(lines[i], lines[j]);
            if (pt.x >= 0 && pt.y >= 0)
                corners.push_back(pt);
        }
    }

    vector<Point2f> approx;
    approxPolyDP(Mat(corners), approx, arcLength(Mat(corners), true) * 0.02, true);

    if (approx.size() != 4)
    {
        cout << "The object is not quadrilateral!" << endl;
        return -1;
    }

    // Get mass center
    for (int i = 0; i < corners.size(); i++)
        center += corners[i];
    center *= (1. / corners.size());

    sortCorners(corners, center);

    // Draw lines
    for (int i = 0; i < lines.size(); i++)
    {
        Vec4i v = lines[i];
        line(dst, Point(v[0], v[1]), Point(v[2], v[3]), CV_RGB(0,255,0));
    }

    // Draw corner points
    circle(dst, corners[0], 3, CV_RGB(255,0,0), 2);
    circle(dst, corners[1], 3, CV_RGB(0,255,0), 2);
    circle(dst, corners[2], 3, CV_RGB(0,0,255), 2);
    circle(dst, corners[3], 3, CV_RGB(255,255,255), 2);

    // Draw mass center
    circle(dst, center, 3, CV_RGB(255,255,0), 2);

    // Calculate corresponding points for corner points
    Mat quad = Mat::zeros(src.rows, src.cols/2, CV_8UC3);

    vector<Point2f> quad_pts;
    quad_pts.push_back(Point2f(0, 0));
    quad_pts.push_back(Point2f(quad.cols, 0));
    quad_pts.push_back(Point2f(quad.cols, quad.rows));
    quad_pts.push_back(Point2f(0, quad.rows));

    // Draw correspondig points
    circle(dst, quad_pts[0], 3, CV_RGB(255,0,0), 2);
    circle(dst, quad_pts[1], 3, CV_RGB(0,255,0), 2);
    circle(dst, quad_pts[2], 3, CV_RGB(0,0,255), 2);
    circle(dst, quad_pts[3], 3, CV_RGB(255,255,255), 2);

    Mat transmtx = getPerspectiveTransform(corners, quad_pts);
    warpPerspective(src, quad, transmtx, quad.size());

    // Create windows and display results
    namedWindow("Original Image", CV_WINDOW_AUTOSIZE );
    namedWindow("Selected Points", CV_WINDOW_AUTOSIZE );
    namedWindow("Corrected Perspertive", CV_WINDOW_AUTOSIZE );

    imshow("Original Image", src);
    imshow("Selected Points", dst);
    imshow("Corrected Perspertive", quad);

    waitKey(); // Wait for key press
    return 0;  // End
}
