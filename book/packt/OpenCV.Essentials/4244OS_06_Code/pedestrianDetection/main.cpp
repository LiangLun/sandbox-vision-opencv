#include "opencv2/core/core.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char *argv[]){
    CascadeClassifier cascade(argv[1]);
    if (cascade.empty())
        return -1;

    VideoCapture vid(argv[2]);
    if (!vid.isOpened()){
        cout<<"Error. The video cannot be opened."<<endl;
        return -1;
    }

    namedWindow("Pedestrian Detection");
    Mat frame;
    while(1) {
        if (!vid.read(frame))
            break;

        Mat frame_gray;
        if(frame.channels()>1){
            cvtColor( frame, frame_gray, CV_BGR2GRAY );
            equalizeHist( frame_gray, frame_gray );
        }else{
            frame_gray = frame;
        }

        vector<Rect> pedestrians;
        cascade.detectMultiScale( frame_gray, pedestrians,
                                  1.1, 2, 0, Size(30, 30),
                                  Size(150, 150) );

        for( size_t i = 0; i < pedestrians.size(); i++ ) {
            Point center( pedestrians[i].x +
                          pedestrians[i].width*0.5,
                          pedestrians[i].y +
                          pedestrians[i].height*0.5 );
            ellipse( frame, center,
                     Size( pedestrians[i].width*0.5,
                     pedestrians[i].height*0.5), 0, 0, 360,
                     Scalar( 255, 0, 255 ), 4, 8, 0 );
        }

        imshow("Pedestrian Detection", frame);
        if(waitKey(100) >= 0)
            break;
    }
    return 0;
}
