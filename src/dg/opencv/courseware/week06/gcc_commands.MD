
test_corner_detect.cpp/test_diy_harris.cpp/test_diy_shitomasi.cpp

g++ -o [target file] [source file] -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lstdc++
g++ -o [target file] [source file] -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lstdc++ -I /usr/local/include/ -L /usr/local/lib


test_optical_flow.cpp
```bash
# g++ -o [target file] [source file] -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_video -lopencv_videoio -lstdc++
g++ -o test_optical_flow test_optical_flow.cpp -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_video -lopencv_videoio -lstdc++
# g++ -o [target file] [source file] -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_video -lopencv_videoio -lstdc++ -I /usr/local/include/ -L /usr/local/lib
```

kalman.cpp
```bash
# g++ -o [target file] [source file] -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_video -lopencv_tracking -lstdc++
g++ -o kalman kalman.cpp -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_video -lopencv_tracking -lstdc++
# g++ -o [target file] [source file] -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_video -lopencv_tracking -lstdc++ -I /usr/local/include/ -L /usr/local/lib
```

camshift.cpp
```bash
# g++ -o [target file] [source file] -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_video -lopencv_videoio -lopencv_tracking -lstdc++
g++ -o camshiftdemo camshiftdemo.cpp -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_video -lopencv_videoio -lopencv_tracking -lstdc++
# g++ -o camshiftdemo camshiftdemo.cpp -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_video -lopencv_videoio -lopencv_tracking -lstdc++ -I /usr/local/include/ -L /usr/local/lib
```



