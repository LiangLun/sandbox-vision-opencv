#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/gpu/gpu.hpp"
#include "opencv2/nonfree/gpu.hpp"

using namespace std;
using namespace cv;

int main( int argc, char** argv )
{
    Mat img_template_cpu = imread( argv[1],IMREAD_GRAYSCALE);
    gpu::GpuMat img_template;
    img_template.upload(img_template_cpu);

    //Detect keypoints and compute descriptors of the template
    gpu::SURF_GPU surf;
    gpu::GpuMat keypoints_template, descriptors_template;

    surf(img_template,gpu::GpuMat(),keypoints_template, descriptors_template);

    //Matcher variables
    gpu::BFMatcher_GPU matcher(NORM_L2);   

    //VideoCapture from the webcam
    gpu::GpuMat img_frame;
    gpu::GpuMat img_frame_gray;
    Mat img_frame_aux;
    VideoCapture cap;
    cap.open(0);
    if (!cap.isOpened()){
        cerr << "cannot open camera" << endl;
        return -1;
    }
    int nFrames = 0;
    uint64 totalTime = 0;
    //main loop
    for(;;){
        int64 start = getTickCount();
        cap >> img_frame_aux;
        if (img_frame_aux.empty())
            break;
        img_frame.upload(img_frame_aux);
        cvtColor(img_frame,img_frame_gray, CV_BGR2GRAY);

        //Step 1: Detect keypoints and compute descriptors
        gpu::GpuMat keypoints_frame, descriptors_frame;
        surf(img_frame_gray,gpu::GpuMat(),keypoints_frame, descriptors_frame);

        //Step 2: Match descriptors
        vector<vector<DMatch> >matches;
        matcher.knnMatch(descriptors_template,descriptors_frame,matches,2);

        //Step 3: Filter results
        vector<DMatch> good_matches;
        float ratioT = 0.7;
        for(int i = 0; i < (int) matches.size(); i++)
        {
            if((matches[i][0].distance < ratioT*(matches[i][1].distance)) && ((int) matches[i].size()<=2 && (int) matches[i].size()>0))
            {
                good_matches.push_back(matches[i][0]);
            }
        }
        // Step 4: Download results
        vector<KeyPoint> keypoints1, keypoints2;
        vector<float> descriptors1, descriptors2;
        surf.downloadKeypoints(keypoints_template, keypoints1);
        surf.downloadKeypoints(keypoints_frame, keypoints2);
        surf.downloadDescriptors(descriptors_template, descriptors1);
        surf.downloadDescriptors(descriptors_frame, descriptors2);

        //Draw the results
        Mat img_result_matches;
        drawMatches(img_template_cpu, keypoints1, img_frame_aux, keypoints2, good_matches, img_result_matches);
        imshow("Matching a template", img_result_matches);

        int64 time_elapsed = getTickCount() - start;
        double fps = getTickFrequency() / time_elapsed;
        totalTime += time_elapsed;
        nFrames++;
        cout << "FPS : " << fps << endl;

        int key = waitKey(30);
        if (key == 27)
            break;;
    }
    double meanFps = getTickFrequency() / (totalTime / nFrames);
    cout << "Mean FPS: " << meanFps << endl;

    return 0;
}
