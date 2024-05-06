//  https://doc.cgal.org/latest/Manual/tuto_reconstruction.html
#include <iostream>
#include <fstream>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Point_set_3.h>
#include <CGAL/draw_point_set_3.h>
//#include <CGAL/Point_set_3/IO/XYZ.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef Kernel::FT FT;
typedef Kernel::Point_3 Point;
typedef Kernel::Vector_3 Vector;
typedef CGAL::Point_set_3<Point> Point_set;

int main(int argc, char** argv) {
    
    Point_set points;
    std::string fname = argc==1?CGAL::data_file_path("../cgal/cgal/Data/data/points_3/kitten.xyz") : argv[1];
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " [input.xyz/off/ply/las]" << std::endl;
        std::cerr <<"Running " << argv[0] << " data/kitten.xyz -1\n";
    }
    std::ifstream stream (fname, std::ios_base::binary);
    if (!stream)
    {
        std::cerr << "Error: cannot read file " << fname << std::endl;
        return EXIT_FAILURE;
    }
    stream >> points;
    std::cout << "Read " << points.size () << " point(s)" << std::endl;
    if (points.empty())
        return EXIT_FAILURE;

    CGAL::draw(points);

    return 0;
}