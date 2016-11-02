#include "opencv2/core.hpp"
#include "opencv2/ml.hpp"

using namespace std;
using namespace cv;
using namespace cv::ml;

static void help()
{
    printf("\nThis program demonstrated the use of OpenCV's decision tree function for learning and predicting data\n"
           "Usage :\n"
           "./mushroom <path to agaricus-lepiota.data>\n"
           "\n"
           "The sample demonstrates how to build a decision tree for classifying mushrooms.\n"
           "It uses the sample base agaricus-lepiota.data from UCI Repository, here is the link:\n"
           "\n"
           "Newman, D.J. & Hettich, S. & Blake, C.L. & Merz, C.J. (1998).\n"
           "UCI Repository of machine learning databases\n"
           "[http://www.ics.uci.edu/~mlearn/MLRepository.html].\n"
           "Irvine, CA: University of California, Department of Information and Computer Science.\n"
           "\n"
           "// loads the mushroom database, which is a text file, containing\n"
           "// one training sample per row, all the input variables and the output variable are categorical,\n"
           "// the values are encoded by characters.\n\n");
}

static bool read_num_class_data( const string& filename, int var_count, Mat* _data, Mat* _responses )
{
    const int M = 1024;
    char buf[M+2];

    Mat el_ptr(1, var_count, CV_32F);
    int i;
    vector<int> responses;

    _data->release();
    _responses->release();

    FILE* f = fopen( filename.c_str(), "rt" );
    if( !f )
    {
        cout << "Could not read the database " << filename << endl;
        return false;
    }

    for(;;)
    {
        char* ptr;
        if( !fgets( buf, M, f ) || !strchr( buf, ',' ) )
            break;
        responses.push_back((int)buf[0]);
        ptr = buf+2;
        for( i = 0; i < var_count; i++ )
        {
            int n = 0;
            sscanf( ptr, "%f%n", &el_ptr.at<float>(i), &n );
            ptr += n + 1;
        }
        if( i < var_count )
            break;
        _data->push_back(el_ptr);
    }
    fclose(f);
    Mat(responses).copyTo(*_responses);

    cout << "The database " << filename << " is loaded.\n";

    return true;
}

static Ptr<TrainData> prepare_train_data(const Mat& data, const Mat& responses, int ntrain_samples)
{
    Mat sample_idx = Mat::zeros( 1, data.rows, CV_8U );
    Mat train_samples = sample_idx.colRange(0, ntrain_samples);
    train_samples.setTo(Scalar::all(1));

    int nvars = data.cols;
    Mat var_type( nvars + 1, 1, CV_8U );
    var_type.setTo(Scalar::all(VAR_ORDERED));
    var_type.at<uchar>(nvars) = VAR_CATEGORICAL;

    return TrainData::create(data, ROW_SAMPLE, responses, noArray(), sample_idx, noArray(), var_type);
}

static void test(const Ptr<StatModel>& model,
                                     const Mat& data, const Mat& responses,
                                     int ntrain_samples, int rdelta)
{
    int i, nsamples_all = data.rows;
    double train_hr = 0, test_hr = 0;

    // compute prediction error on train and test data
    for( i = 0; i < nsamples_all; i++ )
    {
        Mat sample = data.row(i);

        float r = model->predict( sample );
        r = std::abs(r + rdelta - responses.at<int>(i)) <= FLT_EPSILON ? 1.f : 0.f;

        if( i < ntrain_samples )
            train_hr += r;
        else
            test_hr += r;
    }

    test_hr /= nsamples_all - ntrain_samples;
    train_hr = ntrain_samples > 0 ? train_hr/ntrain_samples : 1.;

    printf( "Recognition rate: train = %.1f%%, test = %.1f%%\n",
            train_hr*100., test_hr*100. );
}

int main( int argc, char** argv )
{
    const char* base_path = argc >= 2 ? argv[1] : "agaricus-lepiota.data";
    
    help();

    Mat data;
    Mat responses;
    bool ok = read_num_class_data( base_path, 16, &data, &responses );
    if( !ok )
        return ok;

    Ptr<NormalBayesClassifier> model;

    int nsamples_all = data.rows;
    int ntrain_samples = (int)(nsamples_all*0.8);

    // create classifier by using <data> and <responses>
    cout << "Training the classifier ...\n";
    Ptr<TrainData> tdata = prepare_train_data(data, responses, ntrain_samples);
    model = NormalBayesClassifier::create();
    model->train(tdata);
    cout << endl;

    test(model, data, responses, ntrain_samples, 0);
    
    return 0;
}