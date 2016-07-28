//
//  main.cpp
//  Click-to-Go
//
//  Created by Giray Yıllıkçı on 17/06/15.
//  Copyright (c) 2015 Giray Yıllıkçı. All rights reserved.
//

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

#include "blob.h"
#include "BlobContour.h"
#include "BlobLibraryConfiguration.h"
#include "BlobOperators.h"
#include "BlobProperties.h"
#include "BlobResult.h"
#include "ComponentLabeling.h"

#include <math.h>
#include <stdio.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>

#define PI 3.14159265
#define USB_SERIAL_PORT "/dev/tty.usbmodem1411"

using namespace std;
using namespace cv;


void robot_mouse_callback( int event, int x, int y, int flags, void* param );

int robot_min_x=0, robot_min_y=0, robot_max_x=0, robot_max_y=0;
int robot_center_x=0,robot_center_y=0;
float robot_angle=0.0;
bool is_destination_availible=false;
int destination_x=0, destination_y=0;
float destination_angle=0.0;
int port_fd;

int init_serial_input (char * port) {
    int fd = 0;
    struct termios options;
    
    fd = open(port, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1)
        return fd;
    fcntl(fd, F_SETFL, 0);    // clear all flags on descriptor, enable direct I/O
    tcgetattr(fd, &options);   // read serial port options
    // enable receiver, set 8 bit data, ignore control lines
    options.c_cflag |= (CLOCAL | CREAD | CS8);
    // disable parity generation and 2 stop bits
    options.c_cflag &= ~(PARENB | CSTOPB);
    // set the new port options
    tcsetattr(fd, TCSANOW, &options);
    return fd;
}


Mat redFilter(const Mat& src)
{
    assert(src.type() == CV_8UC3);
    
    Mat redOnly;
    inRange(src, Scalar(0, 0, 50), Scalar(35, 30, 160), redOnly);
    
    return redOnly;
}

Mat blueFilter(const Mat& src)
{
    assert(src.type() == CV_8UC3);
    
    Mat blueOnly;
    inRange(src, Scalar(70, 35, 15), Scalar(160, 90, 60), blueOnly);
    
    return blueOnly;
}


int main(int argc, char** argv)
{
    port_fd = init_serial_input(USB_SERIAL_PORT);

    
    if (port_fd>0)
        printf("We're connected to serial controller\r\n");
    else
    {
        printf("Serial connect error\r\n");
        return 0;
    }
    
    int erosion_size = 1;
    
    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;
    
    Mat element = getStructuringElement( MORPH_ELLIPSE,
                                        Size( 2*erosion_size + 1, 2*erosion_size+1 ) );
    
    Mat frame;
    cap >> frame;
    
    printf("Initializing loop\r\n");
    
    while(1)
    {
        cap >> frame;
        cap >> frame;
        
        if(frame.data == NULL)
        {
            printf("Frame is NULL!\r\n");
            break;
        }
        
        printf("Setting mouse callback\r\n");
        cvSetMouseCallback("frame", robot_mouse_callback, (void*)&frame);
        
        
        Mat blueImg = blueFilter(frame);
        erode(blueImg,blueImg,element);
        dilate( blueImg, blueImg, element );
        
        
        Mat redImg = redFilter(frame);
        erode(redImg,redImg,element);
        dilate( redImg, redImg, element );
        
        CBlobResult blobs;
        IplImage* img = new IplImage(redImg);
        printf("Calculating blobs for red circle\r\n");
        blobs = CBlobResult( img, NULL, 0 );
        blobs.PrintBlobs((char*) "red_blobs.txt" );
        printf("Filtering blobs\r\n");
        blobs.Filter( blobs, B_INCLUDE, CBlobGetArea(), B_GREATER, 200 );
        blobs.PrintBlobs((char*)"red_filteredblobs.txt" );
        
        int blobNums =  blobs.GetNumBlobs();
        printf("Num of blobs for red circle %d\r\n",blobNums);
        int rx=0,ry=0;
        CvRect redBlobBoundingBox;
        if(blobNums)
        {
            
            CBlob currentBlob = blobs.GetBlob(0);
            redBlobBoundingBox = currentBlob.GetBoundingBox();
            rx = currentBlob.MinX() + (redBlobBoundingBox.width/2);
            ry = currentBlob.MinY() + (redBlobBoundingBox.height/2);
        }
        else
        {
            //printf("No blobs found for red circle");
            //break;
        }
        printf("RED Center x=%d, y=%d\r\n",rx,ry);
        
        printf("Creating img for blue circle\r\n");
        
        img = new IplImage(blueImg);
        blobs = CBlobResult( img, NULL, 0 );
        printf("Filtering blobs\r\n");
        blobs.Filter( blobs, B_INCLUDE, CBlobGetArea(), B_GREATER, 200 );
        printf("Get current blob\r\n");
        
        int bx=0, by=0;
        CvRect blueBlobBoundingBox;
        if(blobs.GetNumBlobs())
        {
            CBlob currentBlob = blobs.GetBlob(0);
            printf("Calculating bounding box\r\n");
            blueBlobBoundingBox = currentBlob.GetBoundingBox();
            bx = currentBlob.MinX() + (blueBlobBoundingBox.width/2);
            by = currentBlob.MinY() + (blueBlobBoundingBox.height/2);
        }
        else
        {
            //printf("No blobs found for red circle");
            //break;
        }
        
        printf("Creating img for red circle\r\n");
        printf("BLUE Center x=%d, y=%d\r\n",bx,by);
        printf("Drawing line of robot");
        line(frame,Point(rx,ry),Point(bx,by),Scalar(0,0,255));
        robot_angle = -atan2(ry - by , rx - bx ) * 180 / PI;
        //float angle = atan2(p1.y - p2.y, p1.x - p2.x).
        
        Mat robot;
        add(redImg,blueImg,robot);
        img = new IplImage(robot);
        blobs = CBlobResult( img, NULL, 0 );
        blobs.PrintBlobs((char*) "robot_blobs.txt" );
        blobs.Filter( blobs, B_INCLUDE, CBlobGetArea(), B_GREATER, 200 );
        blobs.PrintBlobs((char*)"robot_filteredblobs.txt" );
        robot_min_x = 0, robot_min_y=0, robot_max_x=0, robot_max_y=0;
        for(int i=0; i< blobs.GetNumBlobs(); i++)
        {
            CBlob robot_Blob = blobs.GetBlob(i);
            if(robot_min_x == 0 || robot_min_x > robot_Blob.MinX())
            {
                robot_min_x = robot_Blob.MinX();
            }
            
            if(robot_min_y == 0 || robot_min_y > robot_Blob.MinY())
            {
                robot_min_y = robot_Blob.MinY();
            }
            
            if(robot_max_x == 0 || robot_max_x < robot_Blob.MaxX())
            {
                robot_max_x = robot_Blob.MaxX();
            }
            
            if(robot_max_y == 0 || robot_max_y < robot_Blob.MaxY())
            {
                robot_max_y = robot_Blob.MaxY();
            }
            
        };
        
        robot_center_x = (robot_min_x + robot_max_x)/2;
        robot_center_y = (robot_min_y + robot_max_y)/2;
        rectangle(frame,Point(robot_min_x,robot_min_y),Point(robot_max_x,robot_max_y),Scalar(0,255,0));
        
        if(is_destination_availible)
        {
            line(frame, Point(robot_center_x,robot_center_y),Point(destination_x,destination_y),Scalar(255,0,0));
            bool is_rotation_required=true;
            if(destination_angle>robot_angle)
            {
                float diff = destination_angle - robot_angle;
                if(diff > 10.0)
                {
                    write(port_fd, "Left!", 6);
                    printf("left\r\n");
                    is_rotation_required = true;
                }
                else
                {
                    is_rotation_required = false;
                }
            }
            else
            {
                float diff =   robot_angle - destination_angle;
                if(diff > 10.0)
                {
                    write(port_fd, "Right!", 6);
                    printf("right\r\n");
                    is_rotation_required = true;
                }
                else
                {
                    is_rotation_required = false;
                }
            }
            
            if(is_rotation_required == false)
            {
                int x_diff = (robot_center_x - destination_x);
                int y_diff = (robot_center_y - destination_y);
                int distance = sqrt( (x_diff*x_diff) + (y_diff*y_diff) );
                
                if(distance > 50)
                {
                    printf("Go!\r\n");
                    write(port_fd, "Go!", 3);
                }
                else
                {
                    printf("Distance Reached!\r\n");
                    write(port_fd, "Stop!", 6);
                    is_destination_availible = false;
                }
            }
            else
            {
                printf("rotation required!\r\n");
            }
            
        }
        
        
        
        imshow("frame",frame);
        if(waitKey(60) >= 0) break;
    }
        
    return 0;
}



void robot_mouse_callback( int event, int x, int y, int flags, void* param ){
    Mat* image = (Mat*) param;
    
    switch( event ){
        case CV_EVENT_MOUSEMOVE: 
            break;
            
        case CV_EVENT_LBUTTONDOWN:
            printf("Clicked to x=%d, y=%d\r\n",x,y);
            printf("Robot angle = %f degree\r\n", robot_angle);
            is_destination_availible = true;
            destination_x = x;
            destination_y = y;
            destination_angle = -atan2(destination_y - robot_center_y , destination_x - robot_center_x ) * 180 / PI;
            printf("Destination angle = %f degree\r\n", destination_angle);
            
            
            break;
            
        case CV_EVENT_LBUTTONUP:
            
            break;
    }
}
