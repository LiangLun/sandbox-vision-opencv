#include <opencv2/core/core.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

#define MAX_KERNEL_LENGTH  (19)
#define MEAN_THRESH_VAL (1)

int main( int argc, char** argv )
{
 if(argc != 2){cout << "Missing argument"<< endl; return -1;}

 Mat org_image, blurred_image,binary_image,edge_image;
 org_image = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);   
 if(org_image.data==NULL){cout << "Empty image"<< endl; return -1;}


 for (int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2 )
 { 
	GaussianBlur( org_image, blurred_image, Size( i, i ), 0, 0 );
 }   
 threshold(org_image,binary_image,170,255,THRESH_BINARY);
 Canny(blurred_image, edge_image,18,36);

 binary_image += edge_image;

 Scalar mean_scalar, stddev_scalar;
 meanStdDev(binary_image,mean_scalar,stddev_scalar);
 double mean_value, stddev_value;
 mean_value = mean_scalar.val[0];
 stddev_value = stddev_scalar.val[0];

 cout << "mean: "<< mean_value << endl <<"stddev: "<< stddev_value << endl;

 if(mean_value > MEAN_THRESH_VAL)
	cout << "Table is not empty!" << endl;
 else
	cout << "Table is empty!" << endl;

 imwrite("binary_image.png",binary_image);
 return 0;
}

