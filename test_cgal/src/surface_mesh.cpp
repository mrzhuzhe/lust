//  https://doc.cgal.org/latest/Manual/tuto_reconstruction.html
/*
qt for remote
qt.qpa.xcb: could not connect to display 
qt.qpa.plugin: Could not load the Qt platform plugin "xcb" in "" even though it was found.
This application failed to start because no Qt platform plugin could be initialized. Reinstalling the application may fix this problem.
Available platform plugins are: eglfs, linuxfb, minimal, minimalegl, offscreen, vkkhrdisplay, vnc, xcb.
export DISPLAY=:1.0
*/

#include <iostream>
#include <fstream>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Point_set_3.h>
#include <CGAL/draw_point_set_3.h>
#include <CGAL/Point_set_3/IO/XYZ.h>

#include <CGAL/remove_outliers.h>
#include <CGAL/compute_average_spacing.h>
#include <CGAL/grid_simplify_point_set.h>

#include <CGAL/jet_smooth_point_set.h>
#include <CGAL/jet_estimate_normals.h>
#include <CGAL/mst_orient_normals.h>

#include <CGAL/Surface_mesh.h>
#include <CGAL/poisson_surface_reconstruction.h>
#include <CGAL/Advancing_front_surface_reconstruction.h>

#include <CGAL/Scale_space_surface_reconstruction_3.h>
#include <CGAL/Scale_space_reconstruction_3/Jet_smoother.h>
#include <CGAL/Scale_space_reconstruction_3/Advancing_front_mesher.h>

#include <CGAL/Polygon_mesh_processing/polygon_soup_to_polygon_mesh.h>

#include <CGAL/draw_point_set_3.h>
#include <CGAL/draw_surface_mesh.h>

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
    // CGAL::jet_smooth_point_set<CGAL::Sequential_tag> (points, 24);

    // Simplify using a grid of size 2 * average spacing
    typename Point_set::iterator gsim_it = CGAL::grid_simplify_point_set (points, 2. * spacing);
    points.remove(gsim_it, points.end());
    std::cout << points.number_of_removed_points()
                << " point(s) removed after simplification." << std::endl;
    points.collect_garbage();

    /*  https://doc.cgal.org/latest/Point_set_processing_3/group__PkgPointSetProcessing3Algorithms.html#ga96a3738be3b2b9bd1587af78ae10e67a 
        dependence on eigen
    */
    CGAL::jet_smooth_point_set<CGAL::Sequential_tag> (points, 24);

    // normal estimae
    CGAL::jet_estimate_normals<CGAL::Sequential_tag>
      (points, 24); // Use 24 neighbors
    // Orientation of normals, returns iterator to first unoriented point
    typename Point_set::iterator unoriented_points_begin =
      CGAL::mst_orient_normals(points, 24); // Use 24 neighbors
    points.remove (unoriented_points_begin, points.end());
    std::cout << points.number_of_removed_points()
                << " point(s) removed after simplification." << std::endl;

    // reconstruction
    CGAL::Surface_mesh<Point> output_mesh;
    CGAL::poisson_surface_reconstruction_delaunay
      (points.begin(), points.end(),
       points.point_map(), points.normal_map(),
       output_mesh, spacing);

    // advancing front
    typedef std::array<std::size_t, 3> Facet; // Triple of indices
    std::vector<Facet> facets;
    // The function is called using directly the points raw iterators
    CGAL::advancing_front_surface_reconstruction(points.points().begin(),
                                                 points.points().end(),
                                                 std::back_inserter(facets));
    std::cout << facets.size ()
              << " facet(s) generated by reconstruction." << std::endl;

    CGAL::Scale_space_surface_reconstruction_3<Kernel> reconstruct
      (points.points().begin(), points.points().end());
    // Smooth using 4 iterations of Jet Smoothing
    reconstruct.increase_scale (4, CGAL::Scale_space_reconstruction_3::Jet_smoother<Kernel>());
    // Mesh with the Advancing Front mesher with a maximum facet length of 0.5
    reconstruct.reconstruct_surface (CGAL::Scale_space_reconstruction_3::Advancing_front_mesher<Kernel>(0.5));

    //CGAL::draw(points); // there is a bug, need press v
    CGAL::draw(output_mesh);

    // std::ofstream f ("out_poisson.ply", std::ios_base::binary);
    // CGAL::IO::set_binary_mode (f);
    // CGAL::IO::write_PLY(f, output_mesh);
    // f.close ();
    
    /*
    std::ofstream f ("out_sp.off");
    f << "OFF" << std::endl << points.size () << " "
      << reconstruct.number_of_facets() << " 0" << std::endl;
    for (Point_set::Index idx : points)
      f << points.point (idx) << std::endl;
    for (const auto& facet : CGAL::make_range (reconstruct.facets_begin(), reconstruct.facets_end()))
      f << "3 "<< facet[0] << " " << facet[1] << " " << facet[2] << std::endl;
    f.close ();
    */

    // copy points for random access
    /*
    std::vector<Point> vertices;
    vertices.reserve (points.size());
    std::copy (points.points().begin(), points.points().end(), std::back_inserter (vertices));
    CGAL::Surface_mesh<Point> output_mesh;
    CGAL::Polygon_mesh_processing::polygon_soup_to_polygon_mesh (vertices, facets, output_mesh);
    std::ofstream f ("out_af.off");
    f << output_mesh;
    f.close ();
    */
   
    return 0;
}