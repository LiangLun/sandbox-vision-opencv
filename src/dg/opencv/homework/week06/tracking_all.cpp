#include <iostream>

#include "opencv2/core.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/tracking.hpp"
#include "opencv2/imgproc.hpp"

using namespace std;
using namespace cv;

int main(int argc, char* argv[])
{
    if (argc < 2) {
        // std::cout << "exe [video]" << std::endl;
        // return -1;
        cout<<
            " Usage: tracker <video_name>\n"
            " examples:\n"
            " example_tracking_kcf Bolt/img/%04d.jpg\n"
            " example_tracking_kcf faceocc2.webm\n"
            << endl;
        return 0;
    }

    // create a tracker object
    Ptr<Tracker> trackerKCF = Tracker::create("KCF");
    Ptr<Tracker> trackerTLD = Tracker::create("TLD");
    Ptr<Tracker> trackerMEDIANFLOW = Tracker::create("MEDIANFLOW");
    Ptr<Tracker> trackerBOOSTING = Tracker::create("BOOSTING");
    Ptr<Tracker> trackerMIL = Tracker::create("MIL");

    VideoCapture cap(argv[1]);
    if (!cap.isOpened()) {
        cout << "error" << endl;
        return -1;
    }


    Mat frame;
    cap >> frame;

    Rect2d roi = selectROI("tracker", frame);
    Rect2d roiTLD = roi;
    Rect2d roiMEDIANFLOW = roi;
    Rect2d roiBOOSTING = roi;
    Rect2d roiMIL = roi;

    Mat target(frame, roi);
    imwrite("target.jpeg", target);
    cout << "(x, y, width, height) = (" << roi.x << "," << roi.y << "," << roi.width << "," << roi.height << ")" << endl;

    if (roi.width == 0 || roi.height == 0)
        return -1;

    // track init
    trackerKCF->init(frame, roi);
    trackerTLD->init(frame, roiTLD);
    trackerMEDIANFLOW->init(frame, roiMEDIANFLOW);
    trackerBOOSTING->init(frame, roiBOOSTING);
    trackerMIL->init(frame, roiMIL);

    // Tracker color setting
    Scalar colorkcf = Scalar(0, 255, 0);
    Scalar colortld = Scalar(0, 255, 255);
    Scalar colormedianflow = Scalar(0, 0, 255);
    Scalar colorboosting = Scalar(255, 255, 0);
    Scalar colormit = Scalar(255, 0, 255);

    // writer video
    double fps = cap.get(CV_CAP_PROP_FPS);
    Size size = Size(cap.get(CV_CAP_PROP_FRAME_WIDTH), cap.get(CV_CAP_PROP_FRAME_HEIGHT));
    const int fourcc = VideoWriter::fourcc('X', 'V', 'I', 'D');
    std::string filename = "output.avi";
    VideoWriter writer(filename, fourcc, fps, size);

    while (1) {
        cap >> frame;
        if (frame.empty())
            break;

        // update
        trackerKCF->update(frame, roi);
        trackerTLD->update(frame, roiTLD);
        trackerMEDIANFLOW->update(frame, roiMEDIANFLOW);
        trackerBOOSTING->update(frame, roiBOOSTING);
        trackerMIL->update(frame, roiMIL);

        // draw rectangle
        rectangle(frame, roi, colorkcf, 1, 1);
        rectangle(frame, roiTLD, colortld, 1, 1);
        rectangle(frame, roiMEDIANFLOW, colormedianflow, 1, 1);
        rectangle(frame, roiBOOSTING, colorboosting, 1, 1);
        rectangle(frame, roiMIL, colormit, 1, 1);

        putText(frame, "- KCF", Point(5, 20), FONT_HERSHEY_SIMPLEX, .5, colorkcf, 1, CV_AA);
        putText(frame, "- TLD", Point(65, 20), FONT_HERSHEY_SIMPLEX, .5, colortld, 1, CV_AA);
        putText(frame, "- MEDIANFLOW", Point(125, 20), FONT_HERSHEY_SIMPLEX, .5, colormedianflow, 1, CV_AA);
        putText(frame, "- BOOSTING", Point(5, 40), FONT_HERSHEY_SIMPLEX, .5, colorboosting, 1, CV_AA);
        putText(frame, "- MIL", Point(115, 40), FONT_HERSHEY_SIMPLEX, .5, colormit, 1, CV_AA);

        imshow("tracker", frame);
        writer << frame;

        int key = waitKey(30);
        if (key == 0x1b)
            break;
    }

    return 0;
}

