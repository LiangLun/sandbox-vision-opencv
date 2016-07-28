#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/gpu/gpu.hpp>
using namespace cv;

int main( int argc, char** argv ){
    if ( argc < 2 ){
        std::cout << "Usage: ./edgesGPU <image>" << std::endl;
        return -1;
    }
    Mat orig = imread(argv[1]);
    gpu::GpuMat g_orig, g_gray, g_dst;
    //Transfer de image data to the GPU
    g_orig.upload(orig);

    gpu::bilateralFilter(g_orig,g_dst,-1,50,7);
    gpu::cvtColor(g_dst,g_gray,COLOR_BGR2GRAY);
    gpu::Canny(g_gray,g_gray,7,20);

    Mat dst;
    //Copy the image back to the CPU memory
    g_gray.download(dst);
    imshow("edgesGPU", dst);
    waitKey(0);

    return 0;
}
