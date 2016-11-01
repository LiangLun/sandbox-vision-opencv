
```bash
g++ -o [target file] [source file] -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_video -lopencv_videoio -lopencv_tracking -lstdc++

camshift_demo.cpp
# ubuntu
g++ -o camshift_demo camshift_demo.cpp -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_video -lopencv_videoio -lopencv_tracking -lstdc++
# mac os
# g++ -o camshift_demo camshift_demo.cpp -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_video -lopencv_videoio -lopencv_tracking -lstdc++ -I /usr/local/include/ -L /usr/local/lib


tracking_demo.cpp
# ubuntu
g++ -o tracking_demo tracking_demo.cpp -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_video -lopencv_videoio -lopencv_tracking -lstdc++
# mac os
# g++ -o tracking_demo tracking_demo.cpp -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_video -lopencv_videoio -lopencv_tracking -lstdc++ -I /usr/local/include/ -L /usr/local/lib


tracking_kcf.cpp
# ubuntu
g++ -o tracking_kcf tracking_kcf.cpp -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_video -lopencv_videoio -lopencv_tracking -lstdc++
# mac os
# g++ -o tracking_kcf tracking_kcf.cpp -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_video -lopencv_videoio -lopencv_tracking -lstdc++ -I /usr/local/include/ -L /usr/local/lib

tracking_all.cpp
# ubuntu
g++ -o tracking_all tracking_all.cpp -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_video -lopencv_videoio -lopencv_tracking -lstdc++
# mac os
# g++ -o tracking_all tracking_all.cpp -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_video -lopencv_videoio -lopencv_tracking -lstdc++ -I /usr/local/include/ -L /usr/local/lib

```
