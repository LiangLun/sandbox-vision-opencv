
```bash

# unzip file
unzip pos.zip -d ./positive_images
unzip neg.zip -d ./negative_images

find ./positive_images/pos_forigner -iname "*.jpg" > positives.txt
find ./negative_images/neg -iname "*.jpg" > negatives.txt

perl createsamples.pl positives.txt negatives.txt samples 1500\
  "opencv_createsamples -bgcolor 0 -bgthresh 0 -maxxangle 1.1\
  -maxyangle 1.1 maxzangle 0.5 -maxidev 40 -w 20 -h 20"

python ./mergevec.py -v samples/ -o samples.vec

opencv_traincascade -data classifier -vec samples.vec -bg negatives.txt\
  -numStages 20 -minHitRate 0.995 -maxFalseAlarmRate 0.5 -numPos 1500\
  -numNeg 5024 -w 20 -h 20 -mode ALL -precalcValBufSize 1024\
  -precalcIdxBufSize 1024

cd ~/opencv-2.4.9/samples/c
chmod +x build_all.sh
./build_all.sh
./facedetect --cascade="~/finished_classifier.xml"

```

```bash
gcc -o facedetect facedetect.cpp -lopencv_core -lopencv_imgproc -lopencv_imgcodecs -lopencv_highgui -lopencv_ml -lopencv_objdetect -lopencv_videoio  -std=c++11 -L/usr/lib64 -lstdc++
./facedetect lena.jpg
./facedetect --cascade="cascade.xml" test.jpg
```


```bash

opencv_createsamples -info ./pos.txt -vec ./pos.vec -num 5000 -w [width] -h [height]
opencv_traincascade -data ./data -vec ./pos.vec -bg ./neg.txt  -info ./pos.txt -vec ./pos.vec -numPos 5000 numNeg 20000 -w 30 -h 30 -numStages 15

opencv_traincascade -data classifier -vec samples.vec -bg negatives.txt -numPos 2971 -numNeg 2561 -featureType HAAR -mode ALL

gcc -o facedetect facedetect.cpp -lopencv_core -lopencv_imgproc -lopencv_imgcodecs -lopencv_highgui -lopencv_ml -lopencv_objdetect -lopencv_videoio  -std=c++11 -L/usr/lib64 -lstdc++
./facedetect lena.jpg


numPos =< (N-S) / (numStage + minHitRate - minHitRate * numStage)
opencv_traincascade -data hoge -vec hoge.vec -bg negative.txt -numPos 1000 -numNeg 3000 -numStages 5 -w 30 -h 30 -minHitRate 0.995 -maxFalseAlarmRate 0.4

(5024-1024)/(20+0.999-0.999*20)
4000/

opencv_createsamples.exe -img ./pos/ooo.jpg -vec ./vec/ooo.vec -num 50 -bgcolor 255 -maxidev 40 -maxxangle 0.8 -maxyangle 0.8 -maxzangle 0.5


vec-file number >= (numPos + (numStages-1) * (1 – minHitRate) * numPos) + S

vec-file number  就是我们的正样本总数5085，numstages=20,minHitRate=0.98,S 表示负样本总数也就是等于2177，这样我们可以就差numPos<=2628.9,所以我们这里的numPos应该去2628，这样正确地输入格式应该是这样的：

opencv_traincascade -data classifier -vec positives.vec -bg negative.txt -numPos 2628 -numNeg 2177 -numStages 20 -precalcValBufSize 5000 -precalcIdxBufSize 5000 -minHitRate 0.98 -maxFalseAlarmRate 0.5 -weightTrimRat 0.95 -w 24 -h 24 -mode ALL

opencv_traincascade -data classifier -vec sample.vec -bg negative.txt -numPos 2628 -numNeg 2177 -numStages 20 -precalcValBufSize 5000 -precalcIdxBufSize 5000 -minHitRate 0.98 -maxFalseAlarmRate 0.5 -weightTrimRat 0.95 -w 24 -h 24 -mode ALL

find positive_images -iname "*.jpg" > positives.txt
find negative_images -iname "*.jpg" > negatives.txt

opencv_createsamples -info positives.txt -vec positives.vec -bg negatives.txt -num 5397 -maxidev 40 -maxxangle 1.100000 -maxyangle 1.100000 -maxzangle 0.5 -w 20 -h 20

opencv_traincascade -data classifier -vec positives.vec -bg negative.txt -numPos 5085 -numNeg 2177 -numStages 20 -precalcValBufSize 5000 -precalcIdxBufSize 5000 -minHitRate 0.98 -maxFalseAlarmRate 0.5 -weightTrimRat 0.95 -w 24 -h 24 -mode ALL

```

