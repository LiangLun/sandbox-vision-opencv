#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/ml/ml.hpp>

using namespace std;
using namespace cv;
using namespace cv::ml;

bool ReadCifar10DataBatch(const string& dir, const string& batchName, size_t imgCount, Mat& images, Mat& labels)
{
    const int PATCH_SIZE = 32;          //图像块的尺寸: 32*32
    const int N_CHANEL = 3;             //通道数
    const int LINE_LENGTH = PATCH_SIZE * PATCH_SIZE * N_CHANEL + 1;//以字节为单位

    bool isSuccess = false;

    fstream fs(dir + batchName, ios::in | ios::binary);//以二进制方式读取

    if (fs.is_open())
    {
        cout << "成功打开文件: " << batchName << endl;
        char buffer[LINE_LENGTH];
        for (size_t imgIdx = 0; imgIdx < imgCount; imgIdx++)
        {
            fs.read(buffer, LINE_LENGTH);
            int class_label = (int)buffer[0];//类别标签:buffer[0]
            Mat red_img(32, 32, CV_8UC1, &buffer[1]);//红色通道：buffer[1->1024]
            Mat green_img(32, 32, CV_8UC1, &buffer[1025]);//绿色通道：buffer[1025->2048]
            Mat blue_img(32, 32, CV_8UC1, &buffer[2049]);//蓝色通道：buffer[2049->3072]
            vector<Mat> bgrMats = { blue_img, green_img, red_img };//OpenCV的通道顺序是BGR
            Mat rgb_img;
            cv::merge(bgrMats, rgb_img);//RGB通道融合
            cv::Mat gray_img;
            cv::cvtColor(rgb_img,gray_img,CV_BGR2GRAY);
            //将样本和对应的标签加入集合
            // train_data.push_back( float_data.reshape(1,1) ); // add 1 row (flattened image)
            images.push_back(gray_img.reshape(0,1));
            // Mat new_label = Mat::zeros(1, 10, labels.type());
            // new_label(class_label, 1) = 1;
            labels.push_back(class_label);
        }
        images.convertTo(images, CV_32FC1);             // to float
        labels.convertTo(labels, CV_32FC1);             // to float
        isSuccess = true;
    }
    else
    {
        cout << "无法打开文件: " << batchName << endl;
        isSuccess = false;
    }

    fs.close();
    return isSuccess;
}

// int main1(int argc, char* argv[])
// {
//     const string dir = "F:\\cifar-10-binary\\cifar-10-batches-bin\\";
//     const string class_names[10] =
//     {
//         "airplane", "automobile", "bird", "cat", "deer", "dog", "frog", "horse", "ship", "truck"
//     };
//     const string batch_names[6] =
//     {
//         "data_batch_1.bin", "data_batch_2.bin", "data_batch_3.bin",
//         "data_batch_4.bin", "data_batch_5.bin", "test_batch.bin"
//     };

//     size_t ImgCountPerBatch = 10000;
//     vector<Mat> images;
//     vector<int> labels;
//     bool success = ReadCifar10DataBatch(dir, batch_names[2], ImgCountPerBatch, images, labels);
//     if (success)
//     {
//         for (size_t imgIdx = 0; imgIdx < images.size(); imgIdx++)
//         {
//             Mat BigImg; cv::resize(images[imgIdx], BigImg, Size(128, 128));
//             imshow("cifar image", BigImg);
//             cout << "image index: "<<imgIdx<<"---->class label,name :" 
//                 << labels[imgIdx] << "<->" << class_names[labels[imgIdx]] << endl;
//             cv::waitKey(5);
//         }
//     }

//     system("pause");
//     return 0;
// }

int getPredictedClass(const cv::Mat& predictions)
{
  float maxPrediction = predictions.at<float>(0);
  float maxPredictionIndex = 0;
  const float* ptrPredictions = predictions.ptr<float>(0);
  for (int i = 0; i < predictions.cols; i++)
  {
      float prediction = *ptrPredictions++;
      if (prediction > maxPrediction)
      {
          maxPrediction = prediction;
          maxPredictionIndex = i;
      }
  }
  return maxPredictionIndex;
}

int main()
{
    const string dir = "./cifar-10-batches-bin/";
    const string class_names[10] =
    {
        "airplane", "automobile", "bird", "cat", "deer", "dog", "frog", "horse", "ship", "truck"
    };
    const string batch_names[6] =
    {
        "data_batch_1.bin", "data_batch_2.bin", "data_batch_3.bin",
        "data_batch_4.bin", "data_batch_5.bin", "test_batch.bin"
    };

    size_t ImgCountPerBatch = 10000;
    Mat_<float> images;
    Mat_<float> labels;
    bool success = ReadCifar10DataBatch(dir, batch_names[2], ImgCountPerBatch, images, labels);
    // if (success)
    // {
    //     for (size_t imgIdx = 0; imgIdx < images.size(); imgIdx++)
    //     {
    //         Mat BigImg; cv::resize(images[imgIdx], BigImg, Size(128, 128));
    //         imshow("cifar image", BigImg);
    //         cout << "image index: "<<imgIdx<<"---->class label,name :" 
    //             << labels[imgIdx] << "<->" << class_names[labels[imgIdx]] << endl;
    //         cv::waitKey(5);
    //     }
    // }

    // system("pause");
    // return 0;
    // //create random training data
    // Mat_<float> data(100, 100);
    // randn(data, Mat::zeros(1, 1, data.type()), Mat::ones(1, 1, data.type()));

    // //half of the samples for each class
    Mat_<float> responses(labels.rows, 10);
    for (int i = 0; i<labels.rows; ++i)
    {
        for (int j = 0; j<10;++j)
        {
            if (j == labels(i,0))
            {
                responses(i, 1) = 1;
            }
            else
            {
                responses(i, 1) = 0;
            }
        }
    }

    /*
    //example code for just a single response (regression)
    Mat_<float> responses(data.rows, 1);
    for (int i=0; i<responses.rows; ++i)
        responses(i, 0) = i < responses.rows / 2 ? 0 : 1;
    */

    //create the neural network
    Mat_<int> layerSizes(1, 3);
    layerSizes(0, 0) = images.cols;
    layerSizes(0, 1) = 20;
    layerSizes(0, 2) = responses.cols;

    Ptr<ANN_MLP> network = ANN_MLP::create();
    network->setLayerSizes(layerSizes);
    network->setActivationFunction(ANN_MLP::SIGMOID_SYM, 0.1, 0.1);
    network->setTrainMethod(ANN_MLP::BACKPROP, 0.1, 0.1);
    
    cout << "create train data" << endl;
    Ptr<TrainData> trainData = TrainData::create(images, ROW_SAMPLE, responses);

    cout << "start train ..." << endl;
    network->train(trainData);
    cout << "network train complete ..." << endl;
    if (network->isTrained())
    {
        // printf("Predict one-vector:\n");
        Mat result;
        // network->predict(Mat::ones(1, data.cols, data.type()), result);
        // cout << result << endl;

        printf("Predict training data:\n");
        for (int i=0; i<images.rows; ++i)
        {
            network->predict(images.row(i), result);
            cout << getPredictedClass(output) << responses.row(i) << endl;
        }
    }

    return 0;
}
