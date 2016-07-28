#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/nonfree.hpp"

using namespace std;
using namespace cv;

int main( int argc, char** argv )
{
    Mat img_orig = imread( argv[1],IMREAD_GRAYSCALE);
    Mat img_fragment = imread( argv[2], IMREAD_GRAYSCALE);
    if(img_orig.empty() || img_fragment.empty())
    {
        cerr << " Failed to load images." << endl;
        return -1;
    }

    //Step 1: Detect keypoints using SURF Detector
     vector<KeyPoint> keypoints1, keypoints2;
     Ptr<FeatureDetector> detector = FeatureDetector::create("SURF");

     detector->detect(img_orig, keypoints1);
     detector->detect(img_fragment, keypoints2);

     //Step 2: Compute descriptors using SURF Extractor
     Ptr< DescriptorExtractor > extractor = DescriptorExtractor::create("SURF");
     Mat descriptors1, descriptors2;

     extractor->compute(img_orig, keypoints1, descriptors1);
     extractor->compute(img_fragment, keypoints2, descriptors2);

     //Step 3: Match descriptors using a FlannBased Matcher
     Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("FlannBased");
     vector<DMatch> matches12;
     vector<DMatch> matches21;
     vector<DMatch> good_matches;

     matcher->match(descriptors1, descriptors2, matches12);
     matcher->match(descriptors2, descriptors1, matches21);

     //Step 4: Filter results using cross-checking
     for( size_t i = 0; i < matches12.size(); i++ )
     {
         DMatch forward = matches12[i];
         DMatch backward = matches21[forward.trainIdx];
         if( backward.trainIdx == forward.queryIdx )
             good_matches.push_back( forward );
     }

     //Draw the results
     Mat img_result_matches;
     drawMatches(img_orig, keypoints1, img_fragment, keypoints2, good_matches, img_result_matches);
     imshow("Matching SURF Descriptors", img_result_matches);

     waitKey(0);

     return 0;
 }
