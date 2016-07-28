#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

int main(int argc, char* argv[]) {

    // Declare input image and retina output buffers
    Mat src, retinaOutput_parvo, retinaOutput_magno;

     src =  imread("starry_night.jpg", 1); // load image in RGB

     // create a retina instance with default parameters setup
     Ptr< Retina> myRetina;

     //allocate "classical" retina :
      myRetina = new  Retina(src.size());

     // save default retina parameters file
      myRetina->write("RetinaDefaultParameters.xml");

     // the retina parameters may be reload using method "setup"
     // uncomment to load parameters if file exists
     // myRetina->setup("RetinaSpecificParameters.xml");
     myRetina->clearBuffers();

     // several iteration of the filter may be done
       for( int iter = 1; iter < 6; iter++ ){
            // run retina filter
            myRetina->run(src);
            // Retrieve and display retina output
            myRetina->getParvo(retinaOutput_parvo);
            myRetina->getMagno(retinaOutput_magno);

            // Create windows and display results
            namedWindow("Source Image", 0 );
            namedWindow("Retina Parvo", 0 );
            namedWindow("Retina Magno", 0 );

            imshow("Source Image", src);
            imshow("Retina Parvo", retinaOutput_parvo);
            imshow("Retina Magno", retinaOutput_magno);
         }

     cout<<"Retina demo end"<< endl;   // Program end message
     waitKey();
     return 0;
}
