#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <iostream>

using namespace cv;
using namespace std;

/*
检测作业素材中“algori_hw.jpg”中的直线，
在中间字母中的直线用绿色标记，字母以外的直线用蓝色标记，上传效果图。
*/

static void help()
{
    cout << "\nThis program demonstrates line finding with the Hough transform.\n"
            "Usage:\n"
            "./houghlines <image_name>, Default is ./algori_hw.jpg\n" << endl;
}

int main(int argc, char** argv)
{
    cv::CommandLineParser parser(argc, argv,
        "{help h||}{@image|./algori_hw.jpg|}"
    );
    if (parser.has("help"))
    {
        help();
        return 0;
    }
    string filename = parser.get<string>("@image");
    if (filename.empty())
    {
        help();
        cout << "no image_name provided" << endl;
        return -1;
    }
    Mat src = imread(filename, 0);
    if(src.empty())
    {
        help();
        cout << "can not open " << filename << endl;
        return -1;
    }

    Mat dst, cdst;
    Canny(src, dst, 50, 200, 3);
    cvtColor(dst, cdst, COLOR_GRAY2BGR);

#if 0
    vector<Vec2f> lines;
    HoughLines(dst, lines, 1, CV_PI/180, 100, 0, 0 );

    for( size_t i = 0; i < lines.size(); i++ )
    {
        float rho = lines[i][0], theta = lines[i][1];
        Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        pt1.x = cvRound(x0 + 1000*(-b));
        pt1.y = cvRound(y0 + 1000*(a));
        pt2.x = cvRound(x0 - 1000*(-b));
        pt2.y = cvRound(y0 - 1000*(a));
        line( cdst, pt1, pt2, Scalar(0,0,255), 3, CV_AA);
    }
#else

    // sub image 01
    CvSize size01 = cvSize(40,50);//区域大小
    cvSetImageROI(cdst,cvRect(60,70,size01.width, size01.height));//设置源图像ROI
    IplImage* dst01 = cvCreateImage(size01,cdst->depth,cdst->nChannels);//创建目标图像
    cvCopy(cdst,dst01); //复制图像
    cvResetImageROI(dst01);//源图像用完后，清空ROI

    // sub image 02
    CvSize size02 = cvSize(40,50);//区域大小
    cvSetImageROI(cdst,cvRect(60,70,size02.width, size02.height));//设置源图像ROI
    IplImage* dst02 = cvCreateImage(size02,cdst->depth,cdst->nChannels);//创建目标图像
    cvCopy(cdst,dst02); //复制图像
    cvResetImageROI(dst02);//源图像用完后，清空ROI

    // sub image 01
    CvSize size03= cvSize(40,50);//区域大小
    cvSetImageROI(cdst,cvRect(60,70,size03.width, size03.height));//设置源图像ROI
    IplImage* dst03 = cvCreateImage(size03,cdst->depth,cdst->nChannels);//创建目标图像
    cvCopy(cdst,dst03); //复制图像
    cvResetImageROI(dst03);//源图像用完后，清空ROI

    // all detected to red lines
    vector<Vec4i> lines;
    HoughLinesP(dst, lines, 1, CV_PI/180, 50, 50, 10 );
    for( size_t i = 0; i < lines.size(); i++ )
    {
        Vec4i l = lines[i];
        line( cdst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, LINE_AA);
    }
#endif
    imshow("source", src);
    imshow("detected lines", cdst);

    waitKey();

    return 0;
}
