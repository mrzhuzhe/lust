#include <iostream>
#include <opencv2/opencv.hpp>
#include <random>

void draw_line(cv::Mat& image, cv::Mat& topk_mat, std::vector<int>& vec_px, std::vector<int>& vec_py,int size) {
   //  cv::Point p1(0, 0);
   //  cv::Point p2(0, 0);
   int index = 0;
   for (int i=0;i<size;i++){
      for (int j=0;j<2;j++){
        index = topk_mat.at<float>(i, j);
        cv::Point p1(vec_px.at(i), vec_py.at(i));
        cv::Point p2(vec_px.at(index), vec_py.at(index));
        cv::line(image, p1, p2 , cv::Scalar(255, 255, 255));
      }
   }   
}

int main(){
    int rows = 700;
    int cols = rows;

    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(0, rows); // define the range

    // initial
    cv::Mat image(rows, cols, CV_32FC3);
    cv::cvtColor(image, image, cv::COLOR_RGB2BGR);    
    std::vector<int> vec_px, vec_py;
    int n = 40; // 80 point
    for (int i=0;i<n;i++){
      int p1x = distr(gen) , p1y = distr(gen), p2x = distr(gen), p2y = distr(gen);
      std::cout << "p1x p1y " << p1x << " " << p1y << " p2x p2y " << p2x << " " << p2y << " " << std::endl; // generate numbers
      cv::Point p1(p1x, p1y);
      cv::Point p2(p2x, p2y);
      vec_px.push_back(p1x);
      vec_py.push_back(p1y);
      vec_px.push_back(p2x);
      vec_py.push_back(p2y);
      //cv::line(image, p1, p2 , cv::Scalar(255, 255, 255));
      cv::circle(image, p1, 0, cv::Scalar(255, 255, 255), 5);
      cv::circle(image, p2, 0, cv::Scalar(255, 255, 255), 5);
    }

    // cal distance
    int size = vec_px.size();
    cv::Mat dis_mat(size, size, CV_32F);
    for (int i=0;i<size;i++){
      for (int j=0;j<size;j++){
        dis_mat.at<float>(i, j) = std::pow(vec_px[i] - vec_px[j], 2) + std::pow(vec_py[i] - vec_py[j], 2);
      }
    }
    //std::cout << dis_mat << std::endl;
    
    // get topk 
    cv::Mat top2_mat(size, 2, CV_32F);
    float min1 = 0, min2 =0;
    int min1_index = 0, min2_index =0;
    for (int i=0;i<size;i++){
      min1 = 9999999999999999, min2 = 99999999999999999;
      for (int j=0;j<size;j++){
        if (i == j) {
          continue;
        }
        if (dis_mat.at<float>(i, j) < min2 ) {
          min2 = dis_mat.at<float>(i, j);
          min2_index = j;
        }
        if (dis_mat.at<float>(i, j) < min1 ) {
          min2 = min1;
          min1 = dis_mat.at<float>(i, j);
          min2_index = min1_index;
          min1_index = j;
        }
        // top 1
        top2_mat.at<float>(i, 0) = min1_index;
        // top 2
        top2_mat.at<float>(i, 1) = min2_index;  
      }
    }
    //std::cout << "\n" <<  top2_mat << std::endl;

    draw_line(image, top2_mat, vec_px, vec_py, size);
    
    int key = 0;
    while(key != 27)
    {
        cv::imshow("image", image);
        key = cv::waitKey(10);
    }
    return 0;
}