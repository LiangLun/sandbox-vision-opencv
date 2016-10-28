test.cpp
g++ -o [target file] [source file] -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lstdc++

// test1.cpp
# ubuntu
g++ -o test1 test1.cpp -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lstdc++
# mac os
g++ -o test1 test1.cpp -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lstdc++ -I /usr/local/include/ -L /usr/local/lib
# run
./test1

// test2.cpp
# ubuntu
g++ -o test2 test2.cpp -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lstdc++
# mac os
g++ -o test2 test2.cpp -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lstdc++  -I /usr/local/include/ -L /usr/local/lib
# run
./test2
