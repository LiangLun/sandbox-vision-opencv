#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;
 
      
Mat src,dst;  
      
    void on_mouse( int event, int x, int y, int flags, void* ustc)  
    {  
        //src = imread("Lena.jpg");    
          
        static Point pre_pt(-1,-1);  
        static Point cur_pt(-1,-1);  
        CvFont font;  
        cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.5, 0.5, 0, 1, CV_AA);  
        char temp[50];  
          
        Vec3b intensity = src.at<Vec3b>(Point(x, y));  
        if( event == CV_EVENT_LBUTTONDOWN )  
        {  
            dst.copyTo(src);      
            // sprintf(temp,"(%d,%d,%d,%d,%d)",x,y,intensity.val[0],intensity.val[1],intensity.val[2]);
            sprintf(temp,"(%d,%d)",x,y);
            pre_pt = cvPoint(x,y);  
            putText(src,temp, pre_pt, FONT_HERSHEY_SIMPLEX, 0.5,cvScalar(0,0, 0, 255),1,8);  
            circle( src, pre_pt, 3,cvScalar(255,0,0,0) ,CV_FILLED, CV_AA, 0 );  
            imshow( "src", src );  
            src.copyTo(dst);  
        }  
        else if( event == CV_EVENT_MOUSEMOVE && (flags & CV_EVENT_FLAG_LBUTTON))  
        {  
            dst.copyTo(src);  
            // sprintf(temp,"(%d,%d,%d,%d,%d)",x,y,intensity.val[0],intensity.val[1],intensity.val[2]);
            sprintf(temp,"(%d,%d)",x,y);
            cur_pt = cvPoint(x,y);        
            putText(src,temp, cur_pt,FONT_HERSHEY_SIMPLEX, 0.5,cvScalar(0,0, 0, 255),1,8);  
            line(src, pre_pt, cur_pt, cvScalar(0,255,0,0), 1, CV_AA, 0 );  
            imshow( "src", src );  
        }  
        else if( event == CV_EVENT_LBUTTONUP )  
        {  
            dst.copyTo(src);  
            // sprintf(temp,"(%d,%d,%d,%d,%d)",x,y,intensity.val[0],intensity.val[1],intensity.val[2]);
            sprintf(temp,"(%d,%d)",x,y);
            cur_pt = Point(x,y);          
            putText(src,temp, cur_pt, FONT_HERSHEY_SIMPLEX, 0.5,cvScalar(0,0, 0, 255),1,8);  
            circle( src, cur_pt, 3,cvScalar(255,0,0,0) ,CV_FILLED, CV_AA, 0 );  
            line( src, pre_pt, cur_pt, cvScalar(0,255,0,0), 1, CV_AA, 0 );  
            imshow( "src", src );  
            src.copyTo(dst);  
        }  
    }  
      
int main()  
{  
        src = imread("algori_hw.jpg");   
          
        src.copyTo(dst);  
        cvNamedWindow("src",CV_WINDOW_AUTOSIZE );  
        cvSetMouseCallback( "src", on_mouse, 0 );  
          
        imshow("src",src);  
        cvWaitKey(0);   
        cvDestroyAllWindows();  
        return 0;  
}  