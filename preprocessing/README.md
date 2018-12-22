## 圖片前處理 - 編譯方法 ##
### 安裝OpenCV ###
    sudo apt-get install libopencv-dev
### 安裝Cmake ###
    sudo apt install cmake
### 使用cmake產生Makefile並執行make ###
    cmake .
    make
OR

    cmake . && make

## 圖片前處理 - 使用方法 ##
### Serial版本 ###
    ./preprocess_serial
### Pthread版本 ###
    ./preprocess_pthread [number_of_cores]
### OpenMP版本 ###
    ./preprocess_openmp
    
如果要測試Scalability

    taskset -c 0[,1,2,3] ./preprocess_openmp
### 最後會輸出 output.csv 其內容格式為 ###
    檔名 R G B (換行)
    e.g., ukbench00000.jpg 135 147 148