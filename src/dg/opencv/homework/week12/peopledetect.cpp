#include <iostream>
#include <fstream>
#include <stdexcept>
#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/video.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/ml/ml.hpp>

using namespace cv;
using namespace cv::ml;
using namespace std;

#define PosSamNum 100    //正样本个数  
#define NegSamNum 100   //负样本个数  

std::string train_image_folder = "./";
std::string test_image_folder  = "./";

const char* keys =
{
    "{ help h      |                     | print help message }"
    "{ image i     |                     | specify input image}"
    "{ camera c    |                     | enable camera capturing }"
    "{ video v     | ./vtest.avi   | use video as input }"
    "{ directory d |                     | images directory}"
};

static void detectAndDraw(const HOGDescriptor &hog, Mat &img)
{
    vector<Rect> found, found_filtered;
    double t = (double) getTickCount();
    // Run the detector with default parameters. to get a higher hit-rate
    // (and more false alarms, respectively), decrease the hitThreshold and
    // groupThreshold (set groupThreshold to 0 to turn off the grouping completely).
    hog.detectMultiScale(img, found, 0, Size(8,8), Size(32,32), 1.05, 2);
    t = (double) getTickCount() - t;
    cout << "detection time = " << (t*1000./cv::getTickFrequency()) << " ms" << endl;

    for(size_t i = 0; i < found.size(); i++ )
    {
        Rect r = found[i];

        size_t j;
        // Do not add small detections inside a bigger detection.
        for ( j = 0; j < found.size(); j++ )
            if ( j != i && (r & found[j]) == r )
                break;

        if ( j == found.size() )
            found_filtered.push_back(r);
    }

    for (size_t i = 0; i < found_filtered.size(); i++)
    {
        Rect r = found_filtered[i];

        // The HOG detector returns slightly larger rectangles than the real objects,
        // so we slightly shrink the rectangles to get a nicer output.
        r.x += cvRound(r.width*0.1);
        r.width = cvRound(r.width*0.8);
        r.y += cvRound(r.height*0.07);
        r.height = cvRound(r.height*0.8);
        rectangle(img, r.tl(), r.br(), cv::Scalar(0,255,0), 3);
    }
}

void train(){

    std::cout<<"start traning..."<<std::endl;

    std::ifstream train_pos_file(train_image_folder + "pos.lst");
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
    cv::Ptr<cv::ml::SVM> svm = SVM::create();
    svm->setType(cv::ml::SVM::C_SVC);
    svm->setKernel(cv::ml::SVM::LINEAR);
    svm->setTermCriteria(cv::TermCriteria(cv::TermCriteria::MAX_ITER, 100, 1e-6));  
    svm->train(sample_features,cv::ml::ROW_SAMPLE,sample_labels);

    //保存SVM分类器
    std::cout<<"start saving svm file..."<<std::endl;
    svm->save("SVM_HOG.xml");

    std::cout<<"finish training..."<<std::endl;
    return;
}

void test_image(std::string test_image_name){
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
    cv::Ptr<cv::ml::SVM> svm = SVM::create();
    svm = SVM::load("./SVM_HOG.xml");
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

vector<float> load_lear_model(const char* model_file)
{
     vector<float>  detector;
     FILE *modelfl;
     if ((modelfl = fopen (model_file, "rb")) == NULL)
     {
         cout<<"Unable to open the modelfile"<<endl;
         return detector;
     }
 
     char version_buffer[10];
     if (!fread (&version_buffer,sizeof(char),10,modelfl))
     {
         cout<<"Unable to read version"<<endl;
         return detector;
     }
 
     if(strcmp(version_buffer,"V6.01"))
     {
        cout<<"Version of model-file does not match version of svm_classify!"<<endl;
        return detector;
     }
     // read version number
     int version = 0;
     if (!fread (&version,sizeof(int),1,modelfl))
     {
         cout<<"Unable to read version number"<<endl;
         return detector;
     }
 
     if (version < 200)
     {
         cout<<"Does not support model file compiled for light version"<<endl;
         return detector;
      }
 
     long kernel_type;
     fread(&(kernel_type),sizeof(long),1,modelfl);   
 
     {// ignore these
        long poly_degree;
        fread(&(poly_degree),sizeof(long),1,modelfl);
 
        double rbf_gamma;
        fread(&(rbf_gamma),sizeof(double),1,modelfl);
 
        double  coef_lin;
        fread(&(coef_lin),sizeof(double),1,modelfl);
        double coef_const;
        fread(&(coef_const),sizeof(double),1,modelfl);
 
        long l;
        fread(&l,sizeof(long),1,modelfl);
        char* custom = new char[l];
        fread(custom,sizeof(char),l,modelfl);
        delete[] custom;
    }
 
    long totwords;
    fread(&(totwords),sizeof(long),1,modelfl);
 
    {// ignore these
        long totdoc;
        fread(&(totdoc),sizeof(long),1,modelfl);
 
        long sv_num;
        fread(&(sv_num), sizeof(long),1,modelfl);
    }
 
    double linearbias = 0.0;
    fread(&linearbias, sizeof(double),1,modelfl);
 
    if(kernel_type == 0) { /* linear kernel */
        /* save linear wts also */
        double* linearwt = new double[totwords+1];
        int length = totwords;
        fread(linearwt, sizeof(double),totwords+1,modelfl);
 
        for(int i = 0;i<totwords;i++){
            float term = linearwt[i];
            detector.push_back(term);
        }
        float term = -linearbias;
        detector.push_back(term);
        delete [] linearwt;
 
    } else {
        cout<<"Only supports linear SVM model files"<<endl;
    }
 
    fclose(modelfl);
    return detector;
 
}

void get_svm_detector(const Ptr<SVM>& svm, vector< float > & hog_detector )
{
    // get the support vectors
    Mat sv = svm->getSupportVectors();
    const int sv_total = sv.rows;
    // get the decision function
    Mat alpha, svidx;
    double rho = svm->getDecisionFunction(0, alpha, svidx);

    CV_Assert( alpha.total() == 1 && svidx.total() == 1 && sv_total == 1 );
    CV_Assert( (alpha.type() == CV_64F && alpha.at<double>(0) == 1.) ||
               (alpha.type() == CV_32F && alpha.at<float>(0) == 1.f) );
    CV_Assert( sv.type() == CV_32F );
    hog_detector.clear();

    hog_detector.resize(sv.cols + 1);
    memcpy(&hog_detector[0], sv.ptr(), sv.cols*sizeof(hog_detector[0]));
    hog_detector[sv.cols] = (float)-rho;
}

int test_video(std::string video_filename)
{

    //加载SVM
    cv::Ptr<cv::ml::SVM> svm = SVM::create();
    svm = SVM::load("./SVM_HOG.xml");
    //如果文件不存在则先训练
    if(svm.empty()){
        train();
    }

    Mat svecsmat = svm -> getSupportVectors();//svecsmat元素的数据类型为float
    int svdim = svm -> getVarCount();//特征向量位数
    int numofsv = svecsmat.rows;

    // alphamat和svindex必须初始化，否则getDecisionFunction()函数会报错
    Mat alphamat = Mat::zeros(numofsv, svdim, CV_32F);
    Mat svindex = Mat::zeros(1, numofsv,CV_64F);

    Mat Result;
    double rho = svm ->getDecisionFunction(0, alphamat, svindex);
    alphamat.convertTo(alphamat, CV_32F);//将alphamat元素的数据类型重新转成CV_32F
    Result = -1 * alphamat * svecsmat;//float

    vector<float> vec;
    for (int i = 0; i < svdim; ++i)
    {
        vec.push_back(Result.at<float>(0, i));
    }
    vec.push_back(rho);

    // Set the trained svm to my_hog
    // vector< float > hog_detector;
    // get_svm_detector( svm, hog_detector );

    //saving HOGDetectorForOpenCV.txt
    // ofstream fout("HOGDetectorForOpenCV.txt");
    // for (int i = 0; i < vec.size(); ++i)
    // {
    //     fout << vec[i] << endl;
    // }

    HOGDescriptor hog;
    // HOGDescriptor hog(Size(768, 576), Size(16,16), Size(8,8), Size(8,8), 9);
    // std::cout << hog.winSize << std::endl;
    // std::cout << vec.size() << std::endl;
    // std::cout << svdim << std::endl;
    // std::cout << vec.getMat().rows << std::endl;
    // std::cout << vec.getMat().cols << std::endl;
    // hog.setSVMDetector(vec);
    vector<float> hog_detector;
    get_svm_detector(svm, hog_detector);
    hog.setSVMDetector(hog_detector);

    // hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
    // std::string svm_model = "./SVM_HOG.xml";
    // vector<float> detector = load_lear_model(svm_model.c_str());
    // hog.setSVMDetector(detector);

    namedWindow("people detector", 1);

    //Read from input image files
    vector<String> filenames;
    //Read from video file
    VideoCapture vc;
    Mat frame;


    if (!video_filename.empty())
    {
        vc.open(video_filename.c_str());
        if (!vc.isOpened())
            throw runtime_error(string("can't open video file: " + video_filename));

        vector<String>::const_iterator it_image = filenames.begin();

        for (;;)
        {
            vc >> frame;

            if (frame.empty())
                break;

            detectAndDraw(hog, frame);

            imshow("people detector", frame);
            int c = waitKey( vc.isOpened() ? 30 : 0 ) & 255;
            if ( c == 'q' || c == 'Q' || c == 27)
                break;
        }
    }

    return 0;
}


int main(int argc,char* argv[]){
    // CommandLineParser parser(argc, argv, keys);

    // if (parser.has("help"))
    // {
    //     cout << "\nThis program demonstrates the use of the HoG descriptor using\n"
    //         " HOGDescriptor::hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());\n";
    //     parser.printMessage();
    //     cout << "During execution:\n\tHit q or ESC key to quit.\n"
    //         "\tUsing OpenCV version " << CV_VERSION << "\n"
    //         "Note: camera device number must be different from -1.\n" << endl;
    //     return 0;
    // }

    // HOGDescriptor hog;
    // hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
    // namedWindow("people detector", 1);

    // string pattern_glob = "";
    // string video_filename = "./vtest.avi";
    // int camera_id = -1;
    // if (parser.has("directory"))
    // {
    //     pattern_glob = parser.get<string>("directory");
    // }
    // else if (parser.has("image"))
    // {
    //     pattern_glob = parser.get<string>("image");
    // }
    // else if (parser.has("camera"))
    // {
    //     camera_id = parser.get<int>("camera");
    // }
    // else if (parser.has("video"))
    // {
    //     video_filename = parser.get<string>("video");
    // }

    if(strcmp(argv[1],"train")==0){
        train();
        return 0;
    }
    if(strcmp(argv[1],"test_image")==0){
        if(argc<3){
            std::cout<<"please input test image file."<<std::endl;
            return 1;
        }
        test_image(argv[2]);
        return 0;
    }
    if(strcmp(argv[1],"test_video")==0){
        if(argc<3){
            std::cout<<"please input test image file."<<std::endl;
            return 1;
        }
        test_video(argv[2]);
        return 0;
    }
}

