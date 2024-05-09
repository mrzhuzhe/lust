//  https://doc.cgal.org/latest/Manual/tuto_reconstruction.html
#include <iostream>
#include <fstream>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Point_set_3.h>
#include <CGAL/draw_point_set_3.h>
#include <CGAL/Point_set_3/IO/XYZ.h>

#include <CGAL/remove_outliers.h>
#include <CGAL/compute_average_spacing.h>
#include <CGAL/grid_simplify_point_set.h>


typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef Kernel::FT FT;
typedef Kernel::Point_3 Point;
typedef Kernel::Vector_3 Vector;
typedef CGAL::Point_set_3<Point> Point_set;

int main(int argc, char** argv) {
    
    /*
    const std::string filename = argc > 1 ? argv[1] : CGAL::data_file_path("../cgal/cgal/Data/data/points_3/oni.pwn");
    Point_set point_set;
    if(!CGAL::IO::read_point_set(filename, point_set))
    {
        std::cerr << "Can't read input file " << filename << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "Read " << point_set.size () << " point(s)" << std::endl;
    CGAL::draw(point_set);
    return EXIT_SUCCESS;
    */

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

    //CGAL::draw(points);
    
    typename Point_set::iterator rout_it = CGAL::remove_outliers<CGAL::Sequential_tag>
    (points,
     24, // Number of neighbors considered for evaluation
    points.parameters().threshold_percent (5.0)); // Percentage of points to remove
    points.remove(rout_it, points.end());
    std::cout << points.number_of_removed_points()
                << " point(s) are outliers." << std::endl;
    // Applying point set processing algorithm to a CGAL::Point_set_3
    // object does not erase the points from memory but place them in
    // the garbage of the object: memory can be freed by the user.
    points.collect_garbage();

    // keep only one point per cell
    // Compute average spacing using neighborhood of 6 points
    double spacing = CGAL::compute_average_spacing<CGAL::Sequential_tag> (points, 6);
    // Simplify using a grid of size 2 * average spacing
    typename Point_set::iterator gsim_it = CGAL::grid_simplify_point_set (points, 2. * spacing);
    points.remove(gsim_it, points.end());
    std::cout << points.number_of_removed_points()
                << " point(s) removed after simplification." << std::endl;
    points.collect_garbage();

    return 0;
}