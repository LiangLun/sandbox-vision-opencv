*********************************************************************
This document is protected for "Arduino Computer Vision Programming" book. If you accidentally get these files, please delete them all.
*********************************************************************
To be able to compile and run the codes, OpenCV and libfreenect (with BUILD_CV=ON option) should be installed to your system. (for our case it is Ubuntu 14.04). You can find the instructions on the book chapter 2.
To be able to compile a file kinect_depthmap.cpp, you should navigate to the folder of kinect_depthmap.cpp by "cd" command and you should type

make kinect_depthmap
chmod+x kinect_depthmap

To run the main example, you should type

./kinect_depthmap

Apply same procedure for all files.

in case of any problem please contact with
ozenozkaya@gmail.com, gyillikci@gmail.com

If you have a library link problem please type
export PKG_CONFIG_PATH=/usr/local/lib/libfreenect
export LD_LIBRARY_PATH=/usr/local/lib/

