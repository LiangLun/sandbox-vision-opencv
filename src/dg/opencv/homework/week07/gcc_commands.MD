
homework_svm.cpp/homework_knn.cpp

```bash
# ubuntu
g++ -o homework_svm homework_svm.cpp -lopencv_ml -lopencv_objdetect -lopencv_core -lopencv_imgproc -lopencv_imgcodecs -lopencv_highgui -std=c++11
# mac os
# g++ -o homework_svm homework_svm.cpp -lopencv_ml -lopencv_objdetect -lopencv_core -lopencv_imgproc -lopencv_imgcodecs -lopencv_highgui -std=c++11 -I /usr/local/include/ -L /usr/local/lib

./homework_svm train
./homework_svm test test/person.bmp
./homework_svm test test/D2004-08-17_11h27m30s.jpg
```

