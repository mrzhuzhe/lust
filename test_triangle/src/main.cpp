#include <iostream>
#include <opencv2/opencv.hpp>

int main(){

    cv::Point p1(100, 100);
    cv::Point p2(300, 300);
    cv::Mat image(700, 700, CV_32FC3);
    cv::cvtColor(image, image, cv::COLOR_RGB2BGR);    
    cv::line(image, p1, p2 , cv::Scalar(255, 255, 255));

    int key = 0;
    while(key != 27)
    {
        cv::imshow("image", image);
        key = cv::waitKey(10);
    }
    return 0;
}