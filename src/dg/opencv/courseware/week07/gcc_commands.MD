
test_svm.cpp/test_knn.cpp


```bash
# ubuntu
g++ -o test_svm test_svm.cpp -lopencv_ml -lopencv_objdetect -lopencv_core -lopencv_imgproc -lopencv_imgcodecs -lopencv_highgui -std=c++11
# mac os
# g++ -o test_svm test_svm.cpp -lopencv_ml -lopencv_objdetect -lopencv_core -lopencv_imgproc -lopencv_imgcodecs -lopencv_highgui -std=c++11 -I /usr/local/include/ -L /usr/local/lib

# create test_svm.xml
./test_svm train
./test_svm test train/neg/00000057a.png
./test_svm test train/pos/crop001001c.png
```

```bash
# ubuntu
g++ -o test_knn test_knn.cpp -lopencv_ml -lopencv_objdetect -lopencv_core -lopencv_imgproc -lopencv_imgcodecs -lopencv_highgui -std=c++11
# mac os
# g++ -o test_knn test_knn.cpp -lopencv_ml -lopencv_objdetect -lopencv_core -lopencv_imgproc -lopencv_imgcodecs -lopencv_highgui -std=c++11 -I /usr/local/include/ -L /usr/local/lib

```

