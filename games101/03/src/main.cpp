#include <eigen3/Eigen/Eigen>
#include <iostream>
#include <opencv2/opencv.hpp>


int main(int argc, const char** argv)
{
    float angle = 0;
    bool command_line = false;
    std::string filename = "output.png";

    if (argc == 2)
    {
        command_line = true;
        filename = std::string(argv[1]);
    }

    //rst::rasterizer r(700, 700);

    Eigen::Vector3f eye_pos = {0, 0, 5};
     
    std::cout << "load model firtst" << std::endl;

    return 0;
}