#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/ml/ml.hpp>

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
    CvMLData mlData;
    mlData.read_csv("iris.csv");
    mlData.set_response_idx(4);
    //Select 75% samples as training set and 25% as test set
    CvTrainTestSplit cvtts(0.75f, true);
    //Split the iris dataset
    mlData.set_train_test_split(&cvtts);

    //Get training set
    Mat trainsindex= mlData.get_train_sample_idx();
    cout<<"Number of samples in the trainig set:"<<trainsindex.cols<<endl;
    //Get test set
    Mat testindex=mlData.get_test_sample_idx();
    cout<<"Number of samples in the test set:"<<testindex.cols<<endl;
    cout<<endl;

    //Random Forst parameters
    CvRTParams params = CvRTParams(3, 1, 0, false, 2, 0, false, 0, 100, 0, CV_TERMCRIT_ITER | CV_TERMCRIT_EPS);

    CvRTrees classifierRF;
    //Taining phase
    classifierRF.train(&mlData,params);
    std::vector<float> train_responses, test_responses;

    //Calculate train error
    cout<<"Error on train samples:"<<endl;
    cout<<(float)classifierRF.calc_error( &mlData, CV_TRAIN_ERROR,&train_responses)<<endl;

    //Print train responses
    cout<<"Train responses:"<<endl;
    for(int i=0;i<(int)train_responses.size();i++)
        cout<<i+1<<":"<<(float)train_responses.at(i)<<"  ";
    cout<<endl<<endl;

    //Calculate test error
    cout<<"Error on test samples:"<<endl;
    cout<<(float)classifierRF.calc_error( &mlData, CV_TEST_ERROR,&test_responses)<<endl;

    //Print test responses
    cout<<"Test responses:"<<endl;
    for(int i=0;i<(int)test_responses.size();i++)
        cout<<i+1<<":"<<(float)test_responses.at(i)<<"  ";
    cout<<endl<<endl;

    return 0;
}



