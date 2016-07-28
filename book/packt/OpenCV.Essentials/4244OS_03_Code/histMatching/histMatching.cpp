#include "opencv2/opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

void histMatch(const Mat &reference, const Mat &target, Mat &result)
{
    float const HISTMATCH = 0.000001;

    double min, max;

    vector<Mat> ref_channels;
    split(reference, ref_channels);
    vector<Mat> tgt_channels;
    split(target, tgt_channels);

    int histSize = 256;
    float range[] = {0, 256};
    const float* histRange = { range };
    bool uniform = true;

    // for every channel (B, G, R)
    for ( int i=0 ; i<3 ; i++ )
    {
        Mat ref_hist, tgt_hist;
        Mat ref_hist_accum, tgt_hist_accum;

        // Calculate histograms
        calcHist(&ref_channels[i], 1, 0, Mat(), ref_hist, 1, &histSize, &histRange, uniform, false);
        calcHist(&tgt_channels[i], 1, 0, Mat(), tgt_hist, 1, &histSize, &histRange, uniform, false);

        // Normalize histograms
        minMaxLoc(ref_hist, &min, &max);
        if (max==0) continue;
        ref_hist = ref_hist / max;
        minMaxLoc(tgt_hist, &min, &max);
        if (max==0) continue;
        tgt_hist = tgt_hist / max;

        // Calculate accumulated histograms
        ref_hist.copyTo(ref_hist_accum);
        tgt_hist.copyTo(tgt_hist_accum);

        float * src_cdf_data = ref_hist_accum.ptr<float>();
        float * dst_cdf_data = tgt_hist_accum.ptr<float>();

        for ( int j=1 ; j < 256 ; j++ )
        {
            src_cdf_data[j] = src_cdf_data[j] + src_cdf_data[j-1];
            dst_cdf_data[j] = dst_cdf_data[j] + dst_cdf_data[j-1];
        }

        // Normalize accumulated
        minMaxLoc(ref_hist_accum, &min, &max);
        ref_hist_accum = ref_hist_accum / max;
        minMaxLoc(tgt_hist_accum, &min, &max);
        tgt_hist_accum = tgt_hist_accum / max;

        // result aux
        Mat Mv(1, 256, CV_8UC1);
        uchar * M = Mv.ptr<uchar>();
        uchar last = 0;
        for ( int j=0 ; j < tgt_hist_accum.rows ; j++ )
        {
            float F1 = dst_cdf_data[j];

            for ( uchar k=last ; k < ref_hist_accum.rows ; k++ )
            {
                float F2 = src_cdf_data[k];

                if ( std::abs(F2 - F1) < HISTMATCH ||  F2 > F1 )
                {
                    M[j] = k;
                    last = k;
                    break;
                }
            }
        }

        Mat lut(1, 256, CV_8UC1, M);
        LUT(tgt_channels[i], lut, tgt_channels[i]);
    }

    // merge the 3 channels into the result image
    merge(tgt_channels, result);
}



int main(int argc, char *argv[])
{
    // Read original image and clone it to contain results
    Mat ref = imread("baboon.jpg", CV_LOAD_IMAGE_COLOR );
    Mat tgt = imread("lena.jpg", CV_LOAD_IMAGE_COLOR );
    Mat dst = tgt.clone();

    // Create 3 windows
    namedWindow("Reference", WINDOW_AUTOSIZE);
    namedWindow("Target", WINDOW_AUTOSIZE);
    namedWindow("Result", WINDOW_AUTOSIZE);
    imshow("Reference", ref);
    imshow("Target", tgt);

    histMatch(ref, tgt, dst);
    imshow("Result", dst);

    // Position windows on screen
    moveWindow("Reference", 0,0);
    moveWindow("Target", ref.cols,0);
    moveWindow("Result", ref.cols+tgt.cols,0);

    cout << "Press any key to exit...\n";
    waitKey(); // Wait for key press
    return 0;
}

