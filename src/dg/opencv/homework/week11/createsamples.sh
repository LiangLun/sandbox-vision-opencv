
#!/bin/sh
positive_dir="./samples/positive/"
list=($(find ./samples/positive/ -name "*.png" ))
length of filelist: ${#list[@]}
 
content=""
for file in ${list[@]}
do
        echo ==========================
    name=${file##*/}
    echo filename: $name
 
    arr=${name//__/ }
    arr=($arr)
    echo ${arr[@]}
    echo width: ${arr[3]}, height: ${arr[4]}
 
    line="$name"
    content="$content$name.vec \n"
     
    vecname=${name//./_}
 
    ./opencvTools/opencv_createsamples.exe -vec ./samples/positive/$vecname.vec -bg ./samples/negtive/bg.txt -img $file -w 50 -h 25 -num 5
        echo ==========================
done
 
listfile=$positive_dir"vec_list.txt"
targetfile=$positive_dir"target.vec"
echo $listfile
echo $targetfile
 
find $positive_dir -name "*png.vec" > $listfile
./imageclipper/mergevec.exe $listfile $targetfile  -w 50 -h 25
