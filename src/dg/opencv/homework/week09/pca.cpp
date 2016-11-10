#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/ml/ml.hpp>

using namespace std;
using namespace cv;
using namespace cv::ml;

bool loadData(const string& dir, const string& batchName, size_t imgCount, vector<Mat>& images, vector<int>& labels)
{
    // image block size : 32*32
    const int PATCH_SIZE = 32;
    // chanel count (RGB)
    const int N_CHANEL = 3;
    // byte count per line
    const int LINE_LENGTH = PATCH_SIZE * PATCH_SIZE * N_CHANEL + 1;
    bool isSuccess = false;
    // read by binary
    fstream fs(dir + batchName, ios::in | ios::binary);

    if (fs.is_open())
    {
        cout << "success for open file : " << batchName << endl;
        char buffer[LINE_LENGTH];
        for (size_t imgIdx = 0; imgIdx < imgCount; imgIdx++)
        {
            fs.read(buffer, LINE_LENGTH);
            // class_label : buffer[0]
            // red chanel : buffer[1->1024]
            // green chanel : buffer[1025->2048]
            // blue chanel : buffer[2049->3072]
            int class_label = (int)buffer[0];
            Mat red_chanel(32, 32, CV_8UC1, &buffer[1]);
            Mat green_chanel(32, 32, CV_8UC1, &buffer[1025]);
            Mat blue_chanel(32, 32, CV_8UC1, &buffer[2049]);
            // OpenCV's chanel seq is BGR
            vector<Mat> bgrMats = { blue_chanel, green_chanel, red_chanel };
            Mat rgb_img;
            // merge RGB chanle
            merge(bgrMats, rgb_img);
            Mat gray_img;
            cvtColor(rgb_img, gray_img, CV_RGB2GRAY);
            gray_img = gray_img.reshape(0,1);
            // gray_img.convertTo(gray_img, CV_32FC1);
            images.push_back(gray_img);
            labels.push_back(class_label);
        }
        isSuccess = true;
    }
    else
    {
        cout << "can't load file : " << batchName << endl;
        isSuccess = false;
    }

    fs.close();
    return isSuccess;
}

int getPredictedClass(const Mat& predictions)
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

static  Mat formatImagesForPCA(const vector<Mat> &data)
{
    Mat dst(static_cast<int>(data.size()), data[0].rows*data[0].cols, CV_32F);
    for(unsigned int i = 0; i < data.size(); i++)
    {
        Mat image_row = data[i].clone().reshape(1,1);
        Mat row_i = dst.row(i);
        image_row.convertTo(row_i,CV_32F);
    }
    return dst;
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
    vector<Mat> images;
    Mat images_pca;
    Mat_<float> images_train;
    vector<int> labels_train;
    Mat_<float> images_test;
    vector<int> labels_test;
    bool success = loadData(dir, batch_names[0], ImgCountPerBatch, images, labels_train);
    success = loadData(dir, batch_names[1], ImgCountPerBatch, images, labels_train);
    success = loadData(dir, batch_names[2], ImgCountPerBatch, images, labels_train);
    success = loadData(dir, batch_names[3], ImgCountPerBatch, images, labels_train);
    success = loadData(dir, batch_names[4], ImgCountPerBatch, images, labels_train);
    images_pca = formatImagesForPCA(images);
    PCA pca(images_pca, cv::Mat(), PCA::DATA_AS_ROW, 0.95);
    images_pca.convertTo(images_train, CV_32FC1);
    
    images.clear()
    success = loadData(dir, batch_names[5], ImgCountPerBatch, images, labels_test);
    for(int i=0; i<images.size(); ++i)
        images_test.push_back(images[i]);
    images_test.convertTo(images_test, CV_32FC1);

    Mat_<float> responses(labels_train.size(), 10);
    for (int i = 0; i<labels_train.size(); ++i)
    {
        for (int j = 0; j<10;++j)
        {
            if (j == labels_train[i])
            {
                responses(i, j) = 1;
            }
            else
            {
                responses(i, j) = 0;
            }
        }
    }
    Mat_<float> responses_test(labels_test.size(), 10);
    for (int i = 0; i<labels_test.size(); ++i)
    {
        for (int j = 0; j<10;++j)
        {
            if (j == labels_test[i])
            {
                responses_test(i, j) = 1;
            }
            else
            {
                responses_test(i, j) = 0;
            }
        }
    }

    // Create the neural network
    Mat_<int> layerSizes(1, 3);
    layerSizes(0, 0) = images_train.cols;
    layerSizes(0, 1) = 1000;
    layerSizes(0, 2) = responses.cols;

    Ptr<ANN_MLP> network = ANN_MLP::create();
    network->setLayerSizes(layerSizes);
    network->setActivationFunction(ANN_MLP::SIGMOID_SYM, 0.1, 0.1);
    network->setTrainMethod(ANN_MLP::BACKPROP, 0.1, 0.1);
    
    printf("Create train data ...\n");
    Ptr<TrainData> trainData = TrainData::create(images_train, ROW_SAMPLE, responses);

    printf("Start training ...\n");
    network->train(trainData);
    printf("Complete training ...\n");
    if (network->isTrained())
    {
        Mat result;

        printf("Predict training data:\n");

        // compute prediction error on train data
        double train_hr = 0, test_hr = 0;
        for (int i = 0; i < images_train.rows; i++)
        {
            Mat sample = images_train.row(i);
            network->predict(sample, result);
            float r = getPredictedClass(result) == labels_train[i] ? 1.f : 0.f;
            train_hr += r;
        }
        train_hr /= images_train.rows;

        printf("Predict test data:\n");

        // compute prediction error on test data
        for (int i = 0; i < images_test.rows; i++)
        {
            Mat sample = images_test.row(i);
            network->predict(sample, result);
            float r = getPredictedClass(result) == labels_test[i] ? 1.f : 0.f;
            test_hr += r;
        }
        cout << test_hr << images_test.rows << endl;
        test_hr /= images_test.rows;
    
        printf("accuracy: train = %.1f%%, test = %.1f%%\n", train_hr*100., test_hr*100.);

        cout << test_hr << images_test.rows << endl;
    }

    return 0;
}
