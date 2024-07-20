//  https://github.com/chen0040/cpp-spline/tree/master
#include <opencv2/opencv.hpp>


int main(){
    int rows = 320;
    int cols = rows;
    cv::Mat image(rows, cols, CV_32F);
    
    int key = 0;
    while(key != 27)
    {
        cv::imshow("image", image);
        key = cv::waitKey(10);
    }

    return 0;
}