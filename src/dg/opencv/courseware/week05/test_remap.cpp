 #include "opencv2/highgui/highgui.hpp"
 #include "opencv2/imgproc/imgproc.hpp"
 #include <iostream>
 #include <stdio.h>

 using namespace cv;


 /// Global variables
 Mat src;
 Mat warp_dst;
 const char* remaptrackbarname="remapvalue";
 const char* warprotatetrackbarname="warprotatevalue";
 const char* warpscaletrackbarname="warpscalevalue";
 const char* remap_window = "Remap demo";
 const char* warprotate_window="warprotate demo";
 const int remapmaxcount=4,warprotatemaxcount=360,warpscalemaxcount=10;
 int remapvalue,warprotatevalue=180, warpscalevalue=10;

 /// Function Headers
 void update_map( void );
 void remapcontrol(int,void*);
 void warprotatecontrol(int,void*);
 void warpaffinecontrol();

 /**
 * @function main
 */
 int main( int argc, char** argv )
 {
   /// Load the image
   src = imread( "transform.jpg", 1 );

   ///把原图变小一倍
   resize(src,src,Size(0.5*src.cols,0.5*src.rows));

  /// Create dst, map_x and map_y with the same size as src:


  /// Create window
  namedWindow( remap_window, CV_WINDOW_AUTOSIZE );
  namedWindow(warprotate_window,CV_WINDOW_AUTOSIZE);
  createTrackbar(remaptrackbarname,remap_window,&remapvalue,remapmaxcount,remapcontrol);
  createTrackbar(warprotatetrackbarname,warprotate_window,&warprotatevalue,warprotatemaxcount,warprotatecontrol);
  createTrackbar(warpscaletrackbarname,warprotate_window,&warpscalevalue,warpscalemaxcount,warprotatecontrol); 
  remapcontrol(0,0);
  warpaffinecontrol();
  warprotatecontrol(0,0);
  waitKey();

  return 0;
 }

 /**
 * @function update_map
 * @brief Fill the map_x and map_y matrices with 4 types of mappings
 */
  void remapcontrol(int,void*)
  {
	  Mat  dst, map_x, map_y; 
	  dst.create( src.size(), src.type() );
	  map_x.create( src.size(), CV_32FC1 );
	  map_y.create( src.size(), CV_32FC1 );
	  for( int j = 0; j < src.rows; j++ )
	  { for( int i = 0; i < src.cols; i++ )
	  {
		  switch( remapvalue )
		  {
		  case 0:			  
			  map_x.at<float>(j,i) = i ;
			  map_y.at<float>(j,i) = j ;
			  break;

		  case 1:
			  if( i > src.cols*0.25 && i < src.cols*0.75 && j > src.rows*0.25 && j < src.rows*0.75 )
			  {
				  map_x.at<float>(j,i) = 2*( i - src.cols*0.25 ) + 0.5 ;
				  map_y.at<float>(j,i) = 2*( j - src.rows*0.25 ) + 0.5 ;
			  }
			  else
			  { map_x.at<float>(j,i) = 0 ;
			  map_y.at<float>(j,i) = 0 ;
			  }
			  break;
		  case 2:
			  map_x.at<float>(j,i) = i ;
			  map_y.at<float>(j,i) = src.rows - j ;
			  break;
		  case 3:
			  map_x.at<float>(j,i) = src.cols - i ;
			  map_y.at<float>(j,i) = j ;
			  break;
		  case 4:
			  map_x.at<float>(j,i) = src.cols - i ;
			  map_y.at<float>(j,i) = src.rows - j ;
			  break;
		  } // end of switch
	  }
	  }
	  remap( src, dst, map_x, map_y, CV_INTER_LINEAR, BORDER_CONSTANT, Scalar(0,0, 0) );

	  /// Display results
	  imshow( remap_window, dst );
  }
  void warprotatecontrol(int,void*)
  {
	  Mat warp_rotate_dst;
	  Mat rot_mat( 2, 3, CV_32FC1 );
	  Point center = Point( warp_dst.cols/2, warp_dst.rows/2 );
	  double angle =warprotatevalue-180;
	  double scale =double(warpscalevalue)/10;
	  printf("%f\n",scale);
	  /// 通过上面的旋转细节信息求得旋转矩阵
	  rot_mat = getRotationMatrix2D( center, angle, scale );

	  /// 旋转已扭曲图像
	  warpAffine( warp_dst, warp_rotate_dst, rot_mat, warp_dst.size() );
	  imshow(warprotate_window,warp_rotate_dst);
  }
   void warpaffinecontrol()
	{
		Point2f srcTri[3];
		Point2f dstTri[3];
		Mat warp_mat( 2, 3, CV_32FC1 );
		warp_dst = Mat::zeros( src.rows, src.cols, src.type() );

		srcTri[0] = Point2f( 0,0 );
		srcTri[1] = Point2f( src.cols - 1, 0 );
		srcTri[2] = Point2f( 0, src.rows - 1 );

		dstTri[0] = Point2f( src.cols*0.0, src.rows*0.33 );
		dstTri[1] = Point2f( src.cols*0.85, src.rows*0.25 );
		dstTri[2] = Point2f( src.cols*0.15, src.rows*0.7 );
		warp_mat = getAffineTransform( srcTri, dstTri );
	    warpAffine( src, warp_dst, warp_mat, warp_dst.size() );
	};
