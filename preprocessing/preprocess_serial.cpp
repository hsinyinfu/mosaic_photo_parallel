#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <string>

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

int main(int argc, char* argv[]) {
  // Set the path;
  string path = "./img/";

  // Read image data 
  for(int i = 0 ; i < MAX_IMAGE_NUM ; i++){
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
      return -1;
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

  // Output to output.csv
  std::ofstream outFile("output.csv", std::ios_base::out);

  for(unsigned int i = 0; i < MAX_IMAGE_NUM; i++) {
		outFile << images[i].filename << ' ';
    outFile << images[i].R << ' ';
    outFile << images[i].G << ' ';
    outFile << images[i].B << std::endl;
	}
  
  return 0;
}