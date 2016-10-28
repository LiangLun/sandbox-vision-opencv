// 1. “作业素材”中提供了一个完整的人脸数据集和一份丰富的负样本集，使用本课程讲到的OpenCV中的机器学习方法（SVM或KNN），
// 实现人脸和非人脸图片的判断。素材中的数据库，可以自行分为训练样本和测试样本。
// TIPS：
// 注意正负样本的尺寸，颜色通道要一直，负样本比正样本大的话，只需要在负样本中随机取出一部分就可以，不强制需要使用完所有数据。
// * 能力较好的同学可以尝试在人脸的检测器中使用HAAR或者LBP方法取代课程中用到的HOG方法。 

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

#define PosSamNum 100    //正样本个数  
#define NegSamNum 100   //负样本个数  

std::string train_image_folder = "./";
std::string test_image_folder  = "./";

void train(){

    std::cout<<"start traning..."<<std::endl;

    std::ifstream train_pos_file(train_image_folder + "yale.lst");
    std::ifstream train_neg_file(train_image_folder + "neg.lst");

    std::string image_name; //图片路径

    cv::Mat src;

    //定义HOG检测器
    int descriptor_dim = 0;
    //检测窗口(64,128),块尺寸(16,16),块步长(8,8),cell尺寸(8,8),直方图bin个数9 
    cv::HOGDescriptor hog(cv::Size(64,128),cv::Size(16,16),cv::Size(8,8),cv::Size(8,8),9);//HOG检测器，用来计算HOG描述子的
    std::vector<float> descriptors; //HOG描述子向量


    cv::Mat sample_features; //所有训练样本的特征向量组成的矩阵，行数等于所有样本的个数，列数等于HOG描述子维数  
    cv::Mat sample_labels;  //训练样本的类别向量，行数等于所有样本的个数，列数等于1；1表示有人，-1表示无人  

    //获取训练数据的正样本
    for(int num=0; num<PosSamNum && getline(train_pos_file,image_name);num++){
        image_name = train_image_folder + image_name;
        src = cv::imread(image_name,0);
        
        cv::resize(src,src,cv::Size(96,160)); //统一训练格式

        if(src.empty()){
            std::cout<<"no image in pos image path:"<<image_name<<std::endl;
            exit(0);
        }

        hog.compute(src,descriptors,cv::Size(8,8));

        if(num==0){
            std::cout<<"Init feature size..."<<std::endl;
            descriptor_dim = descriptors.size();
            sample_features = cv::Mat::zeros(PosSamNum+NegSamNum,descriptor_dim,CV_32FC1);
            sample_labels   = cv::Mat::zeros(PosSamNum+NegSamNum,1,CV_32SC1); //注意这里必须要32SC1,不能用32FC1
        }

        for(int i=0;i<descriptor_dim;i++){
            sample_features.at<float>(num,i) = descriptors[i];
        }
        sample_labels.at<int>(num,0) = 1; //正样本类别为1，有人

        std::cout<<"finish Pos "<<num<<" "<<image_name<<std::endl;

    }

    //获取训练数据的负样本
    for(int num=0;num<NegSamNum && getline(train_neg_file,image_name);num++){
        image_name = train_image_folder + image_name;
        src = cv::imread(image_name,0);
        if(src.empty()){
            std::cout<<"no image in neg image path..."<<std::endl;
            exit(0);
        }
        cv::resize(src,src,cv::Size(96,160));

        hog.compute(src,descriptors,cv::Size(8,8));

        for(int i=0;i<descriptor_dim;i++){
            sample_features.at<float>(num+PosSamNum,i) = descriptors[i];
        }
        sample_labels.at<int>(num+PosSamNum,0) = -1; //正样本类别为-1，没有人

        std::cout<<"finish Neg "<<num<<" "<<image_name<<std::endl;
    }


    //训练SVM分类器
    std::cout<<"start svm training..."<<std::endl;
    cv::Ptr<cv::ml::SVM> svm = cv::ml::SVM::create();
    svm->setType(cv::ml::SVM::C_SVC);
    svm->setKernel(cv::ml::SVM::LINEAR);
    svm->setTermCriteria(cv::TermCriteria(cv::TermCriteria::MAX_ITER, 100, 1e-6));  
    svm->train(sample_features,cv::ml::ROW_SAMPLE,sample_labels);

    //保存SVM分类器
    std::cout<<"start saving svm file..."<<std::endl;
    svm->save("test_svm.xml");

    std::cout<<"finish training..."<<std::endl;
    return;
}

void test(std::string test_image_name){
    std::cout<<"start testing..."<<std::endl;

    //加载图片
    cv::Mat test_image = cv::imread(test_image_name,0);
    if(test_image.empty()){
        std::cout<<"Input test image path is error!"<<std::endl;
        return;
    }

    //获取特征并组织为单行的格式
    cv::resize(test_image,test_image,cv::Size(96,160)); //训练样本的尺寸是（96，160），修改训练数据后，要同步修改
    cv::HOGDescriptor hog(cv::Size(64,128),cv::Size(16,16),cv::Size(8,8),cv::Size(8,8),9);
    std::vector<float> descriptors;
    hog.compute(test_image,descriptors,cv::Size(8,8));
    cv::Mat test_feature = cv::Mat::zeros(1,descriptors.size(),CV_32FC1);
    for(int i=0;i<descriptors.size();i++){
        test_feature.at<float>(0,i) = descriptors[i];
    }

    //加载SVM
    cv::Ptr<cv::ml::SVM> svm = cv::ml::SVM::create();
    svm = cv::ml::SVM::load("./test_svm.xml");
    //如果文件不存在则先训练
    if(svm.empty()){
        train();
    }
    //输出结果
    cv::Mat result;
    svm->predict(test_feature,result);
    float res = result.at<float>(0,0);
    if(res==1){
        std::cout<<"Result: 1, 有人！"<<std::endl;
    }else if(res==-1){
        std::cout<<"Result: -1, 没有人！"<<std::endl;
    }
    return;

}

int main(int argc,char* argv[]){
    if(strcmp(argv[1],"train")==0){
        train();
        return 0;
    }
    if(strcmp(argv[1],"test")==0){
        if(argc<3){
            std::cout<<"please input test image file."<<std::endl;
            return 1;
        }
        test(argv[2]);
        return 0;
    }
}


