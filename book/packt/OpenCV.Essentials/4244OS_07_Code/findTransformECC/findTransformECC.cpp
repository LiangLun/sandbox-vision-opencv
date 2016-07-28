#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int findCameraMovement()
{
    // 1-Set the parameters and initializations
    bool finish = false;
    Mat frame;
    Mat initial_frame;
    Mat warp_matrix;
    Mat warped_frame;
    int warp_mode = MOTION_HOMOGRAPHY;
    TermCriteria criteria(TermCriteria::COUNT | TermCriteria::EPS, 50, 0.001);
    VideoCapture capture(0);
    Rect rec(100,50,350,350);   // Initial rectangle
    Mat aux_initial_frame;
    bool follow = false;

    // Check if video camera is opened
    if(!capture.isOpened()) return 1;

    // 2-Initial capture
    cout << "\n Press 'c' key to continue..." << endl;
    while(!follow)
    {
        if(!capture.read(initial_frame)) return 1;
        cvtColor(initial_frame ,initial_frame, COLOR_BGR2GRAY);
        aux_initial_frame = initial_frame.clone();
        rectangle(aux_initial_frame,rec,Scalar(255,255,255),3);
        imshow("Initial frame",aux_initial_frame);
        if (waitKey(1) == 99) follow = true;
    }
    Mat template_frame(rec.width,rec.height,CV_32F);
    template_frame = initial_frame.colRange(rec.x,rec.x+rec.width).rowRange(rec.y,rec.y+rec.height);
    imshow("Template image",template_frame);

    while(!finish)
    {
        cout << "\n Press a key to continue..." << endl;
        waitKey();

        warp_matrix = Mat::eye(3, 3, CV_32F);

        // 3-Read each frame, if possible
        if(!capture.read(frame)) return 1;

        // Convert to gray image
        cvtColor(frame ,frame, COLOR_BGR2GRAY);

        try
        {
            // 4-Use findTransformECC function
            findTransformECC(template_frame, frame, warp_matrix, warp_mode, criteria);

            // 5-Obtain the new perspective
            warped_frame = Mat(template_frame.rows, template_frame.cols, CV_32F);
            warpPerspective (frame, warped_frame, warp_matrix, warped_frame.size(),
                WARP_INVERSE_MAP + WARP_FILL_OUTLIERS);
        }
        catch(Exception e) { cout << "Exception: " << e.err << endl; }

        imshow ("Frame", frame);
        imshow ("Warped frame", warped_frame);

        // Press Esc to finish
        if(waitKey(1) == 27) finish = true;
    }
    capture.release();
    return 0;
}

main()
{
    findCameraMovement();
}
