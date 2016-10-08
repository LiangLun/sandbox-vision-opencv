#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
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
    CommandLineParser parser(argc, argv,
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

    Mat dst, cdst, dst_sub, cdst_sub;
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

    // std::cout<<dst.type()<<std::endl;
    // std::cout<<cdst.type()<<std::endl;
    // std::cout<<CV_8UC1<<std::endl;

    // // 区域大小
    // CvSize size01 = cvSize(40,50);
    // // 设置源图像ROI
    // cvSetImageROI(cdst,cvRect(60,70,size01.width, size01.height));
    // // 创建目标图像
    // IplImage* dst01 = cvCreateImage(size01,cdst->depth,cdst->nChannels);
    // // 复制图像
    // cvCopy(cdst,dst01);
    // // 源图像用完后，清空ROI
    // cvResetImageROI(dst01);

    // all detected to blue lines
    vector<Vec4i> lines;
    HoughLinesP(dst, lines, 1, CV_PI/180, 50, 50, 10 );
    for( size_t i = 0; i < lines.size(); i++ )
    {
        Vec4i l = lines[i];
        line( cdst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255,0,0), 3, LINE_AA);
    }

    // sub image 01 all detected to green lines
    Rect rect_1(330, 370, 200, 50);
    dst_sub = dst(rect_1);
    cvtColor(dst_sub, cdst_sub, COLOR_GRAY2BGR);
    HoughLinesP(dst_sub, lines, 1, CV_PI/180, 50, 50, 10 );
    for( size_t i = 0; i < lines.size(); i++ )
    {
        Vec4i l = lines[i];
        line( cdst_sub, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,255,0), 3, LINE_AA);
    }
    cdst_sub.copyTo(cdst(rect_1));

    // sub image 02 all detected to green lines
    Rect rect_2(280, 415, 410, 110);
    dst_sub = dst(rect_2);
    cvtColor(dst_sub, cdst_sub, COLOR_GRAY2BGR);
    HoughLinesP(dst_sub, lines, 1, CV_PI/180, 50, 50, 10 );
    for( size_t i = 0; i < lines.size(); i++ )
    {
        Vec4i l = lines[i];
        line( cdst_sub, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,255,0), 3, LINE_AA);
    }
    cdst_sub.copyTo(cdst(rect_2));

    // sub image 03 all detected to green lines
    Rect rect_3(430, 530, 140, 260);
    dst_sub = dst(rect_3);
    cvtColor(dst_sub, cdst_sub, COLOR_GRAY2BGR);
    HoughLinesP(dst_sub, lines, 1, CV_PI/180, 50, 50, 10 );
    for( size_t i = 0; i < lines.size(); i++ )
    {
        Vec4i l = lines[i];
        line( cdst_sub, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,255,0), 3, LINE_AA);
    }
    cdst_sub.copyTo(cdst(rect_3));

    // sub image 04 all detected to green lines
    Rect rect_4(100, 780, 510, 199);
    dst_sub = dst(rect_4);
    cvtColor(dst_sub, cdst_sub, COLOR_GRAY2BGR);
    HoughLinesP(dst_sub, lines, 1, CV_PI/180, 50, 50, 10 );
    for( size_t i = 0; i < lines.size(); i++ )
    {
        Vec4i l = lines[i];
        line( cdst_sub, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,255,0), 3, LINE_AA);
    }
    cdst_sub.copyTo(cdst(rect_4));

#endif

    imwrite("detected lines.jpg", cdst);

    imshow("source", src);
    imshow("detected lines", cdst);

    waitKey();

    return 0;
}
