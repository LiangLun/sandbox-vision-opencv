#include <iostream>
#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/ml/ml.hpp>

using namespace std;
using namespace cv;
using namespace cv::ml;

class MySVM : public ml::SVM {
public:
    //获得SVM的决策函数中的alpha数组
    double get_svm_rho() {
        return this->getDecisionFunction(0, svm_alpha, svm_svidx);
    }

    //获得SVM的决策函数中的rho参数,即偏移量

    vector<float> svm_alpha;
    vector<float> svm_svidx;
    float svm_rho;

};


int main() {
    namedWindow("src", 0);
    //检测窗口(64,128),块尺寸(16,16),块步长(8,8),cell尺寸(8,8),直方图bin个数9
    //HOGDescriptor hog(Size(64, 128), Size(16, 16), Size(8, 8), Size(8, 8), 9);//HOG检测器，用来计算HOG描述子的
    int DescriptorDim;//HOG描述子的维数，由图片大小、检测窗口大小、块大小、细胞单元中直方图bin个数决定
    //Ptr svm = ml::SVM::create();
    Ptr <ml::SVM> svm = ml::SVM::load("SVM_HOG_2400PosINRIA_12000Neg_HardExample.xml");
    DescriptorDim = svm->getVarCount();//特征向量的维数，即HOG描述子的维数
    Mat supportVector = svm->getSupportVectors();//支持向量的个数
    int supportVectorNum = supportVector.rows;
    cout << "支持向量个数：" << supportVectorNum << endl;
    //-------------------------------------------------
    vector<float> svm_alpha;
    vector<float> svm_svidx;
    float svm_rho;

    svm_rho = svm->getDecisionFunction(0, svm_alpha, svm_svidx);
    //-------------------------------------------------
    Mat alphaMat = Mat::zeros(1, supportVectorNum, CV_32FC1);//alpha向量，长度等于支持向量个数
    Mat supportVectorMat = Mat::zeros(supportVectorNum, DescriptorDim, CV_32FC1);//支持向量矩阵
    Mat resultMat = Mat::zeros(1, DescriptorDim, CV_32FC1);//alpha向量乘以支持向量矩阵的结果
    supportVectorMat = supportVector;
    ////将alpha向量的数据复制到alphaMat中
    //double * pAlphaData = svm.get_alpha_vector();//返回SVM的决策函数中的alpha向量
    for (int i = 0; i < supportVectorNum; i++) {
        alphaMat.at(0, i) = svm_alpha[i];
    }

    //计算-(alphaMat * supportVectorMat),结果放到resultMat中
    //gemm(alphaMat, supportVectorMat, -1, 0, 1, resultMat);//不知道为什么加负号？
    resultMat = -1 * alphaMat * supportVectorMat;

    //得到最终的setSVMDetector(const vector& detector)参数中可用的检测子
    vector myDetector;
    //将resultMat中的数据复制到数组myDetector中
    for (int i = 0; i < DescriptorDim; i++) {
        myDetector.push_back(resultMat.at(0, i));
    }
    //最后添加偏移量rho，得到检测子
    myDetector.push_back(svm_rho);
    cout << "检测子维数：" << myDetector.size() << endl;
    //设置HOGDescriptor的检测子
    HOGDescriptor myHOG;

    //myHOG.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
    myHOG.setSVMDetector(myDetector);
    //myHOG.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());

    /**************读入图片进行HOG行人检测******************/
    //Mat src = imread("00000.jpg");
    //Mat src = imread("2007_000423.jpg");


    VideoCapture vp("./vtest.avi");

    Mat frame;

    while (true) {
        if (!vp.read(frame)) {
            cout << "读取视频结束" << endl;
            break;
        }
        Mat src = frame;
        vector found, found_filtered;//矩形框数组
        //cout << "进行多尺度HOG人体检测" << endl;
        myHOG.detectMultiScale(src, found, 0, Size(8, 8), Size(32, 32), 1.05, 2);//对图片进行多尺度行人检测
        //cout << "找到的矩形框个数：" << found.size() << endl;

        //找出所有没有嵌套的矩形框r,并放入found_filtered中,如果有嵌套的话,则取外面最大的那个矩形框放入found_filtered中
        for (int i = 0; i < found.size(); i++) {
            Rect r = found[i];
            int j = 0;
            for (; j < found.size(); j++)
                if (j != i && (r & found[j]) == r)
                    break;
            if (j == found.size())
                found_filtered.push_back(r);
        }

        //画矩形框，因为hog检测出的矩形框比实际人体框要稍微大些,所以这里需要做一些调整
        for (int i = 0; i < found_filtered.size(); i++) {
            Rect r = found_filtered[i];
            r.x += cvRound(r.width * 0.1);
            r.width = cvRound(r.width * 0.8);
            r.y += cvRound(r.height * 0.07);
            r.height = cvRound(r.height * 0.8);
            rectangle(src, r.tl(), r.br(), Scalar(0, 255, 0), 3);
        }


        imshow("src", src);
        waitKey(1);//注意：imshow之后必须加waitKey，否则无法显示图像

    }


    system("pause");
}