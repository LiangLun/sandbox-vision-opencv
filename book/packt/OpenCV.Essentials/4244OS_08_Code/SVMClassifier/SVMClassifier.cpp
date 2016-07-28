#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/ml/ml.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
    Mat groups;
    Mat samples;
    vector<KeyPoint> keypoints1;
    //ORB feature detector with 15 interest points
    OrbFeatureDetector detector(15, 1.2f, 2, 31,0, 2, ORB::HARRIS_SCORE, 31);
    Mat descriptors, descriptors2;
    //Surf feature descriptor
    SurfDescriptorExtractor extractor;

    //Training samples
    for(int i=1; i<=56; i++){
        stringstream nn;
        nn <<i<<".png";
        //Read the image to be trained
        Mat img=imread(nn.str());
        cvtColor(img, img, COLOR_BGR2GRAY);
        //Detect interest points
        detector.detect(img, keypoints1);
        //Compute SURF descriptors
        extractor.compute(img, keypoints1, descriptors);
        //Organize and save information in one row
        samples.push_back(descriptors.reshape(1,1));
        keypoints1.clear();
    }

    //Set the labels of each sample
    for(int j=1; j<=56; j++){
        if(j<=14)  groups.push_back(1);
        else if(j>14 && j<=28)  groups.push_back(2);
             else if(j>28 && j<=42)  groups.push_back(3);
                  else groups.push_back(4);
    }

    //Indicate SVM parameters
    CvSVMParams params=CvSVMParams(CvSVM::C_SVC, CvSVM::LINEAR, 0, 1, 0, 1, 0, 0, 0, cvTermCriteria(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS, 100, FLT_EPSILON));

    //Create SVM classifier
    CvSVM classifierSVM;

    //Train classifier
    classifierSVM.train(samples, groups, Mat(), Mat(), params );

    //Test samples
    for(int i=1; i<=10; i++){
        stringstream nn;
        nn <<"unknown"<<i<<".png";
        //Read the image to be tested
        Mat unknown=imread(nn.str());
        cvtColor(unknown, unknown, COLOR_BGR2GRAY);
        //Detect interest points
        detector.detect(unknown, keypoints1);
        //Compute descriptors
        extractor.compute(unknown, keypoints1, descriptors2);
        //Test sample
        float result=classifierSVM.predict(descriptors2.reshape(1,1));
        //Print result
        cout<<nn.str()<<": class "<<result<<endl;
    }
    return 0;
}

