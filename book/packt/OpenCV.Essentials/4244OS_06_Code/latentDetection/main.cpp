#include "opencv2/core/core.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char* argv[]){

    String model = argv[1];
    vector<String> models;
    models.push_back( model );
    vector<String> names;
    names.push_back( "category" );
    LatentSvmDetector detector( models , names);
    if( detector.empty() ) {
        cout << "Model cannot be loaded" << endl;
        return -1;
    }

    String img = argv[2];
    Mat image = imread( img );
    if( image.empty() ){
        cout << "Image cannot be loaded" << endl;
        return -1;
    }

    vector<LatentSvmDetector::ObjectDetection> detections;
    detector.detect( image, detections, 0.1, 1);
    for( size_t i = 0; i < detections.size(); i++ ) {
        Point center( detections[i].rect.x +
                      detections[i].rect.width*0.5,
                      detections[i].rect.y +
                      detections[i].rect.height*0.5 );
        ellipse( image, center, Size( detections[i].rect.width*0.5,
                 detections[i].rect.height*0.5), 0, 0, 360,
                 Scalar( 255, 0, 255 ), 4, 8, 0 );
    }
    imshow( "result", image );
    waitKey(0);
    return 0;
}
