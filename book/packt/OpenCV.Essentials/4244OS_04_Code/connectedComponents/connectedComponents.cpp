#include <opencv2/core/utility.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>

using namespace cv;
using namespace std;

Mat img;
int threshval = 227;

static void on_trackbar(int, void*)
{
    Mat bw = threshval < 128 ? (img < threshval) : (img > threshval);
    Mat labelImage(img.size(), CV_32S);

    Mat stats, centroids;
    int nLabels = connectedComponentsWithStats(bw, labelImage, stats, centroids);

    // Show connected components with random colors
    std::vector<Vec3b> colors(nLabels);
    colors[0] = Vec3b(0, 0, 0);//background
    for(int label = 1; label < nLabels; ++label){
        colors[label] = Vec3b( (rand()&200), (rand()&200), (rand()&200) );
    }
    Mat dst(img.size(), CV_8UC3);
    for(int r = 0; r < dst.rows; ++r){
        for(int c = 0; c < dst.cols; ++c){
            int label = labelImage.at<int>(r, c);
            Vec3b &pixel = dst.at<Vec3b>(r, c);
            pixel = colors[label];
         }
     }
    // Text labels with area of each cc (except background)
    for (int i=1; i< nLabels;i++)
    {
        float a=stats.at<int>(i,CC_STAT_AREA);
        Point org(centroids.at<double>(i,0), centroids.at<double>(i,1));
        String txtarea;
        std::ostringstream buff;
        buff << a;
        txtarea=buff.str();
        putText( dst, txtarea, org,FONT_HERSHEY_COMPLEX_SMALL, 1, Scalar(255,255,255), 1);
    }

    imshow( "Connected Components", dst );
}

int main( int argc, const char** argv )
{
    img = imread("stuff.jpg", 0);
    namedWindow( "Connected Components", 1 );
    createTrackbar( "Threshold", "Connected Components", &threshval, 255, on_trackbar );
    on_trackbar(threshval, 0);

    waitKey(0);
    return 0;
}
