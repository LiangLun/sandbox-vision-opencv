#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/features2d.hpp> //Thanks to Alessandro

using namespace cv;
int main(int argc, const char* argv[])
{
	const Mat input = cv::imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE); //Load as grayscale

    SiftFeatureDetector detector;
    std::vector<KeyPoint> keypoints;
    detector.detect(input, keypoints);

    // Add results to image and save.
    Mat output;
    drawKeypoints(input, keypoints, output);
    imwrite("sift_result.png", output);

    return 0;
}