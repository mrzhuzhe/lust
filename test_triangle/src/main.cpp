#include <iostream>
#include <opencv2/opencv.hpp>
#include <random>

int main(){
    int rows = 700;
    int cols = rows;

    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(0, rows); // define the range

    
    cv::Mat image(rows, cols, CV_32FC3);
    cv::cvtColor(image, image, cv::COLOR_RGB2BGR);    

    int n = 40;
    for (int i=0;i<n;i++){
      int p1x = distr(gen) , p1y = distr(gen), p2x = distr(gen), p2y = distr(gen);
      std::cout << "p1x p1y " << p1x << "" << p1y << " p2x p2y " << p2x << " " << p2y << " " << std::endl; // generate numbers
      cv::Point p1(p1x, p1y);
      cv::Point p2(p2x, p2y);
      cv::line(image, p1, p2 , cv::Scalar(255, 255, 255));
    }
  
    int key = 0;
    while(key != 27)
    {
        cv::imshow("image", image);
        key = cv::waitKey(10);
    }
    return 0;
}