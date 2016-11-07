#include "opencv2/opencv.hpp"
#include <iostream>
using namespace std;
using namespace cv;
using namespace cv::ml;

int main()
{
    const int MAX_CLUSTERS = 5;
    Vec3b colorTab[] =
    {
        Vec3b(0, 0, 255),
        Vec3b(0, 255, 0),
        Vec3b(255, 100, 100),
        Vec3b(255, 0, 255),
        Vec3b(0, 255, 255)
    };
    Mat data, labels;
    Mat pic = imread("./woman.png");
    for (int i = 0; i < pic.rows; i++)
        for (int j = 0; j < pic.cols; j++)
        {
            Vec3b point = pic.at<Vec3b>(i, j);
            Mat tmp = (Mat_<float>(1, 3) << point[0], point[1], point[2]);
            data.push_back(tmp);
        }
    
    int N =3;  //聚成3类
    Ptr<EM> em_model = EM::create();
    em_model->setClustersNumber(N);
    em_model->setCovarianceMatrixType(EM::COV_MAT_SPHERICAL);
    em_model->setTermCriteria(TermCriteria(TermCriteria::COUNT + TermCriteria::EPS, 300, 0.1)); //达到最大迭代次数或者迭代误差小到一定值，应该有系统默认的值
    em_model->trainEM(data, noArray(), labels, noArray());
    
    int n = 0;
    //显示聚类结果，不同的类别用不同的颜色显示
    for (int i = 0; i < pic.rows; i++)
        for (int j = 0; j < pic.cols; j++)
        {
            int clusterIdx = labels.at<int>(n);
            pic.at<Vec3b>(i, j) = colorTab[clusterIdx];
            n++;
        }
    imshow("pic", pic);
    waitKey(0);
    
    return 0;
}