
101_ObjectCategories.tar.gz

https://www.vision.caltech.edu/Image_Datasets/Caltech101/101_ObjectCategories.tar.gz
wget --no-check-certificate --no-cookies --header "Cookie: license=accept-securebackup-cookie" https://www.vision.caltech.edu/Image_Datasets/Caltech101/101_ObjectCategories.tar.gz
tar zxvf 101_ObjectCategories.tar.gz

python opencv_match.py
python opencv_match.py -s pearson -f hash
python opencv_match.py -s pearson -f rgb
# python opencv_match.py -s pearson -f otsu

python opencv_match.py -s hamming -f rgb

