#include "opencv2/core.hpp"
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <vector>
#include <iostream>
#include <iomanip>

using namespace std;
using namespace cv;

int main(int argc, const char * argv[]){

    Mat src = imread(argv[1]);

    vector<Mat> channels;
    computeNMChannels(src, channels);

    //Negative images from RGB channels
    channels.push_back(255-channels[0]);
    channels.push_back(255-channels[1]);
    channels.push_back(255-channels[2]);
    channels.push_back(255-channels[3]);
    for (int c = 0; c < channels.size(); c++){
        stringstream ss;
        ss << "Channel: " << c;
        imshow(ss.str(),channels.at(c));
    }

    Ptr<ERFilter> er_filter1 = createERFilterNM1(
                                   loadClassifierNM1(argv[2]),
                                   16, 0.00015f, 0.13f, 0.2f,
                                   true, 0.1f );
    Ptr<ERFilter> er_filter2 = createERFilterNM2(
                                   loadClassifierNM2(argv[3]),
                                   0.5 );

    vector<vector<ERStat> > regions(channels.size());
    // Apply filters to each channel
    for (int c=0; c<(int)channels.size(); c++){
        er_filter1->run(channels[c], regions[c]);
        er_filter2->run(channels[c], regions[c]);
    }
    for (int c=0; c<(int)channels.size(); c++){
        Mat dst = Mat::zeros( channels[0].rows +
                              2, channels[0].cols + 2, CV_8UC1 );
        // Show ERs
        for (int r=0; r<(int)regions[c].size(); r++)
        {
            ERStat er = regions[c][r];
            if (er.parent != NULL){
                int newMaskVal = 255;
                int flags = 4 + (newMaskVal << 8) +
                                 FLOODFILL_FIXED_RANGE +
                                 FLOODFILL_MASK_ONLY;
                floodFill( channels[c], dst, Point(er.pixel %
                           channels[c].cols,er.pixel /
                           channels[c].cols), Scalar(255), 0,
                           Scalar(er.level), Scalar(0), flags);
            }
        }
        stringstream ss;
        ss << "Regions/Channel: " << c;
        imshow(ss.str(), dst);
    }

    vector<Rect> groups;
    erGrouping( channels, regions, argv[4], 0.5, groups );
    for (int i=(int)groups.size()-1; i>=0; i--)
    {
        if (src.type() == CV_8UC3)
            rectangle( src,groups.at(i).tl(), groups.at(i).br(),
                       Scalar( 0, 255, 255 ), 3, 8 );
        else
            rectangle( src,groups.at(i).tl(), groups.at(i).br(),
                       Scalar( 255 ), 3, 8 );
    }
    imshow("grouping",src);

    waitKey(-1);
    er_filter1.release();
    er_filter2.release();
    regions.clear();
    groups.clear();
}
