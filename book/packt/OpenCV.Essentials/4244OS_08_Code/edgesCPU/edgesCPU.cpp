#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
using namespace cv;

int main( int argc, char** argv ){

    if ( argc < 2 ){
        std::cout << "Usage: ./edgesGPU <image>" << std::endl;
        return -1;
    }
    Mat orig = imread(argv[1]);
    Mat gray, dst;

    bilateralFilter(orig,dst,-1,50,7);
    cvtColor(dst,gray,COLOR_BGR2GRAY);
    Canny(gray,gray,7,20);

    imshow("edgesCPU", gray);
    waitKey(0);

    return 0;
}
