#include <eigen3/Eigen/Eigen>
#include <iostream>
#include <opencv2/opencv.hpp>

#include "rasterizer.hpp"
#include "Triangle.hpp"
#include "OBJ_Loader.h"
#include <boost/dll.hpp>

#define MY_PI 3.1415926

Eigen::Matrix4f get_view_matrix(Eigen::Vector3f eye_pos)
{
    Eigen::Matrix4f view = Eigen::Matrix4f::Identity();

    Eigen::Matrix4f translate;
    translate << 1, 0, 0, -eye_pos[0],
        0, 1, 0, -eye_pos[1],
        0, 0, 1, -eye_pos[2],
        0, 0, 0, 1;

    view = translate * view;

    return view;
}

Eigen::Matrix4f get_model_matrix(float angle)
{
    Eigen::Matrix4f rotation;
    angle = angle * MY_PI / 180.f;
    rotation << cos(angle), 0, sin(angle), 0,
        0, 1, 0, 0,
        -sin(angle), 0, cos(angle), 0,
        0, 0, 0, 1;

    Eigen::Matrix4f scale;
    scale << 2.5, 0, 0, 0,
        0, 2.5, 0, 0,
        0, 0, 2.5, 0,
        0, 0, 0, 1;

    Eigen::Matrix4f translate;
    translate << 1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1;

    return translate * rotation * scale;
}

Eigen::Matrix4f get_projection_matrix(float eye_fov, float aspect_ratio, float zNear, float zFar)
{
    // TODO: Use the same projection matrix from the previous assignments
    Eigen::Matrix4f projection;

    float angel = eye_fov / 180.0 * MY_PI;
    float t = zNear * std::tan(angel / 2);
    float r = t * aspect_ratio;
    float l = -r;
    float b = -t;

    Eigen::Matrix4f MorthoScale(4, 4);
    MorthoScale << 2 / (r - l), 0, 0, 0,
        0, 2 / (t - b), 0, 0,
        0, 0, 2 / (zFar - zNear), 0,
        0, 0, 0, 1;

    Eigen::Matrix4f MorthoPos(4, 4);
    MorthoPos << 1, 0, 0, -(r + l) / 2,
        0, 1, 0, -(t + b) / 2,
        0, 0, 1, -(zNear + zFar) / 2,
        0, 0, 0, 1;

    Eigen::Matrix4f Mpersp2ortho(4, 4);

    Mpersp2ortho << zNear, 0, 0, 0,
        0, zNear, 0, 0,
        0, 0, zNear + zFar, -zNear * zFar,
        0, 0, 1, 0;

    //为了使得三角形是正着显示的，这里需要把透视矩阵乘以下面这样的矩阵
    //参考：http://games-cn.org/forums/topic/%e4%bd%9c%e4%b8%9a%e4%b8%89%e7%9a%84%e7%89%9b%e5%80%92%e8%bf%87%e6%9d%a5%e4%ba%86/
    Eigen::Matrix4f Mt(4, 4);
    Mt << 1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, -1, 0,
        0, 0, 0, 1;

    Mpersp2ortho = Mpersp2ortho * Mt;

    projection = MorthoScale * MorthoPos * Mpersp2ortho * projection;

    return projection;
}

int main(int argc, const char** argv)
{
    boost::filesystem::path exe_path = boost::dll::program_location().parent_path();
    exe_path += "/";

    std::vector<Triangle *> TriangleList;

    float angle = 140.0;
    bool command_line = false;

    std::string filename = exe_path.string() + "output.png";
    objl::Loader Loader;
    std::string obj_path = exe_path.string() + "../../03/models/spot/";

    std::vector<Eigen::Vector3f> pos;

    std::vector<Eigen::Vector3i> ind;

    std::vector<Eigen::Vector3f> cols;
    
    // Load .obj File
    bool loadout = Loader.LoadFile(exe_path.string() + "../../03/models/spot/spot_triangulated_good.obj");
    for (auto mesh : Loader.LoadedMeshes)
    {
        for (int i = 0; i < mesh.Vertices.size(); i += 3)
        {
            Triangle *t = new Triangle();
            for (int j = 0; j < 3; j++)
            {
                pos.push_back(Vector3f(mesh.Vertices[i + j].Position.X, mesh.Vertices[i + j].Position.Y, mesh.Vertices[i + j].Position.Z));                
                cols.push_back(Vector3f(255, 255, 255));
            }
            ind.push_back(Vector3i(i, i+1, i+2));
            TriangleList.push_back(t);
        }
    }
    
    rst::rasterizer r(700, 700);
    Eigen::Vector3f eye_pos = {0, 0, 5};

    auto pos_id = r.load_positions(pos);
    auto ind_id = r.load_indices(ind);
    auto col_id = r.load_colors(cols);

    int key = 0;
    int frame_count = 0;

    while (key != 27) {
        r.clear(rst::Buffers::Color | rst::Buffers::Depth);

        r.set_model(get_model_matrix(angle));

        r.set_view(get_view_matrix(eye_pos));
        //注意这里写入的zNear和zFar是正数，代表着距离，但课程上推导的透视矩阵是坐标，且假定是朝向z负半轴的，所以透视矩阵是需要取反的
        r.set_projection(get_projection_matrix(45, 1, 0.1, 50));

        r.draw(pos_id, ind_id, col_id, rst::Primitive::Triangle);
        
        cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());
        image.convertTo(image, CV_8UC3, 1.0f);
        cv::cvtColor(image, image, cv::COLOR_RGB2BGR);
        cv::imshow("image", image);
        key = cv::waitKey(10);

        std::cout << "frame count: " << frame_count++ << '\n';
    }

    return 0;
}