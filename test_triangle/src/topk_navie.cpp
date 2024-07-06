#include <iostream>
#include <opencv2/opencv.hpp>
#include <random>

void draw_line(cv::Mat& image, cv::Mat& topk_mat, std::vector<int>& vec_px, std::vector<int>& vec_py, int size, int topk) {
   //  cv::Point p1(0, 0);
   //  cv::Point p2(0, 0);
   int index = 0;
   for (int i=0;i<size;i++){
      for (int j=0;j<topk;j++){
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
      //std::cout << "p1x p1y " << p1x << " " << p1y << " p2x p2y " << p2x << " " << p2y << " " << std::endl; // generate numbers
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
    int topk = 10;
    cv::Mat top2_mat(size, topk, CV_32F);
    std::vector<float> mink(topk);
    std::vector<int> indexk(topk);
    for (int i=0;i<size;i++){
      for (int k=0; k < topk; k ++) {
        mink[k] = 999999999999999999.f; 
      }
      for (int j=0;j<size;j++){
        if (i == j) {
          continue;
        }
        for (int k=topk; k >=0; k--) {
          if (dis_mat.at<float>(i, j) < mink[k] ) {
            if (k<topk-1){
              mink[k+1] = mink[k];
              indexk[k+1] = indexk[k];
            }
            mink[k] = dis_mat.at<float>(i, j);
            indexk[k] = j;
          }
        }
        for (int k=0;k<topk;k++) {
          top2_mat.at<float>(i, k) = indexk[k];
        }         
      }
    }
    //std::cout << "\n" <<  top2_mat << std::endl;

    draw_line(image, top2_mat, vec_px, vec_py, size, topk);
    
    int key = 0;
    while(key != 27)
    {
        cv::imshow("image", image);
        key = cv::waitKey(10);
    }
    return 0;
}