
houghcircle.cpp

```bash
# g++ -o [target file] [source file] -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lstdc++
# ubuntu
g++ -o houghcircles houghcircles.cpp -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lstdc++
# mac os
# g++ -o houghcircles houghcircles.cpp -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lstdc++  -I /usr/local/include/ -L /usr/local/lib
```

houghlines.cpp

```bash
# g++ -o [target file] [source file] -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lstdc++
# ubuntu
g++ -o houghlines houghlines.cpp -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lstdc++
# mac os
# g++ -o houghlines houghlines.cpp -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lstdc++  -I /usr/local/include/ -L /usr/local/lib
```

bgfg_segm.cpp

```bash
# g++ -o [target file] [source file] -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_video -lopencv_videoio —lstdc++
# ubutnu
g++ -o bgfg_segm bgfg_segm.cpp -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_video -lopencv_videoio -lstdc++
# mac os
# g++ -o bgfg_segm bgfg_segm.cpp -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_video -lopencv_videoio -lstdc++  -I /usr/local/include/ -L /usr/local/lib

./bgfg_segm --fn=./bike_hw.avi
./bgfg_segm --file_name=./bike_hw.avi

```


