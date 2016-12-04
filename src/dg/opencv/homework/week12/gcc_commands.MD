
wget http://pascal.inrialpes.fr/data/human/INRIAPerson.tar
tar -zxvf INRIAPerson.tar

# create base filename list for pos & neg
ls -1 ./INRIAPerson/96X160H96/Train/pos/ | tr '\n' '\0' | xargs -0 -n 1 basename > pos.lst
create filename list for neg
ls -1 ./INRIAPerson/Train/neg/ | tr '\n' '\0' | xargs -0 -n 1 basename > neg.lst

# create a filename list for pos & neg
ls -d -1 INRIAPerson/96X160H96/Train/pos/*.* > pos.lst
ls -d -1 INRIAPerson/Train/neg/*.* > neg.lst

g++ -o peopledetect peopledetect.cpp -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs -lopencv_objdetect -lopencv_videoio -lopencv_ml -std=c++11
./peopledetect

# create test_svm.xml
./peopledetect train
./peopledetect test_image ./test.jpg
./peopledetect test_video ./vtest.avi

ls /some/path/* | xargs -n1 basename
ls /some/path/*png | xargs -n1 basename
ls /some/path/*png | xargs -n1 basename > pos.lst
ls /some/path/*png | xargs -n1 basename > neg.lst

g++ -o train_HOG train_HOG.cpp -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs -lopencv_objdetect -lopencv_videoio -lopencv_ml -std=c++11
./train_HOG --pd=/INRIA_dataset/ -p=Train/pos.lst --nd=/INRIA_dataset/ -n=Train/neg.lst
./train_HOG --pd=INRIAPerson/ -p=Train/pos.lst --nd=INRIAPerson/ -n=Train/neg.lst
./train_HOG --pd=./INRIAPerson/ -p=./INRIAPerson/Train/pos.lst --nd=./INRIAPerson/ -n=./INRIAPerson/Train/neg.lst

