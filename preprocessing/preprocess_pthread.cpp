#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <pthread.h>

#define MAX_IMAGE_NUM 1000

using namespace cv;
using namespace std;

class Image {
  public:
    string filename;
    unsigned int R;
    unsigned int G;
    unsigned int B;
  
  public:
    Image() : filename(""), R(0), G(0), B(0) {};

    void save_data(string _filename, unsigned int _R, unsigned int _G, unsigned int _B){
      this->filename = _filename;
      this->R = _R;
      this->G = _G;
      this->B = _B;
    }
};

Image images[1000];

long long int loop_times;
long long int number_of_cores;
// Set the path;
string path = "./img/";

void *avg_thread(void *arg){
  int id = *((int *) arg);

  int start = id * loop_times;
  int end = start + loop_times;

  if(id == number_of_cores - 1){
    end = MAX_IMAGE_NUM;
  }

  // Read image data 
  for(int i = start ; i < end ; i++){
    // e.g.: ./img/ukbench00000.jpg
    char img_name[50];
    sprintf(img_name, "ukbench%05d.jpg", i);
    string filename(img_name);
    string filepath = path + filename;

    Mat img_bgr;
    img_bgr = imread( filepath, 1 );

    // Check image exists
    if ( !img_bgr.data ) {
      printf("No image data. filename: %s \n" , filename.c_str());
      return (void *) 0;
    }

    // Calculate the R G B avg;
    unsigned int R_sum = 0;
    unsigned int G_sum = 0;
    unsigned int B_sum = 0;
    for(int r = 0; r < img_bgr.rows; ++r) {
      for(int c = 0; c < img_bgr.cols; ++c) {
        // std::cout << "Pixel at position (x, y) : (" << c << ", " << r << ") =" <<
        // img_bgr.at<Vec3b>(r,c) << std::endl;
        Vec3b pixel = img_bgr.at<Vec3b>(r,c);
        uchar blue = pixel[0];
        B_sum += blue;
        uchar green = pixel[1];
        G_sum += green;
        uchar red = pixel[2];
        R_sum += red;
      }
    }

    unsigned int total_pixel_num = img_bgr.rows * img_bgr.cols;
    unsigned int R_avg = R_sum / total_pixel_num;
    unsigned int G_avg = G_sum / total_pixel_num;
    unsigned int B_avg = B_sum / total_pixel_num;

    images[i].save_data(filename, R_avg, G_avg, B_avg);
  }
}

int main(int argc, char* argv[]) {
  
  if(argc != 2){
    printf("Usage : ./preprocess_pthread [number_of_cores]\n");
    return 0;
  }

  long long int thread;
  number_of_cores = atoll(argv[1]);

  loop_times = MAX_IMAGE_NUM / number_of_cores;

  pthread_t thread_handles[number_of_cores];
  int id[number_of_cores];
  for(thread = 0 ; thread < number_of_cores ; thread++){
    id[thread] = thread;
    pthread_create(&thread_handles[thread], NULL, avg_thread, (void *) &id[thread]);
  }

  for(thread = 0 ; thread < number_of_cores ; thread++){
    pthread_join(thread_handles[thread], NULL);
  }

  // Output to output.csv
  std::ofstream outFile("output.csv", std::ios_base::out);

  for(unsigned int i = 0; i < MAX_IMAGE_NUM; i++) {
    outFile << images[i].filename << ' ';
    outFile << images[i].R << ' ';
    outFile << images[i].G << ' ';
    outFile << images[i].B << std::endl;
  }

  outFile.close();
  
  return 0;
}