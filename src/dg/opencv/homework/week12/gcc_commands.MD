
wget http://pascal.inrialpes.fr/data/human/INRIAPerson.tar
tar -zxvf INRIAPerson.tar

ls /some/path/* | xargs -n1 basename
ls /some/path/*png | xargs -n1 basename
ls /some/path/*png | xargs -n1 basename > pos.lst
ls /some/path/*png | xargs -n1 basename > neg.lst

g++ -o train_HOG train_HOG.cpp -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs -lopencv_objdetect -lopencv_videoio -lopencv_ml -std=c++11
./train_HOG --pd=/INRIA_dataset/ -p=Train/pos.lst --nd=/INRIA_dataset/ -n=Train/neg.lst
./train_HOG --pd=/INRIAPerson/ -p=Train/pos.lst --nd=/INRIAPerson/ -n=Train/neg.lst
./train_HOG --pd=./INRIAPerson/ -p=./INRIAPerson/Train/pos.lst --nd=./INRIAPerson/ -n=./INRIAPerson/Train/neg.lst
