// topologic algorithem is quite good https://www.boost.org/doc/libs/1_83_0/libs/geometry/doc/html/geometry/reference/algorithms.html
// overlap 
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/geometries/geometries.hpp>

#include <vector>
#include <iostream>
#include <fstream>

#include <boost/foreach.hpp>

#include <boost/geometry/io/svg/write_svg.hpp>

namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;

typedef bg::model::point<float, 2, bg::cs::cartesian> point;
typedef bg::model::box<point> box;
typedef std::pair<box, unsigned> value;

#include <boost/assign.hpp>
using namespace boost::assign;


// create the rtree using default constructor
bgi::rtree< value, bgi::quadratic<16> > rtree;

// template <typename Geometry1>
// void create_svg(std::string const& filename, Geometry1 const& a)
template <typename Geometry1, typename Geometry2>
void create_svg(std::string const& filename, Geometry1 const& a, Geometry2 const& b)
{
    typedef typename boost::geometry::point_type<Geometry1>::type point_type;
    std::ofstream svg(filename.c_str());

    boost::geometry::svg_mapper<point_type> mapper(svg, 400, 400);
    mapper.add(a);
    mapper.add(b);

    mapper.map(a, "fill-opacity:0.5;fill:rgb(153,204,0);stroke:rgb(153,204,0);stroke-width:2");
    mapper.map(b, "opacity:0.8;fill:none;stroke:rgb(255,128,0);stroke-width:4;stroke-dasharray:1,7;stroke-linecap:round");
}

int main()
{

    bg::model::polygon<bg::model::d2::point_xy<float> > poly;
    bg::read_wkt("POLYGON((1.5 0.5, 1.5 2.5, 3 3, 3.5 2.5, 3.5 0.5, 1.5 0.5))", poly);
    
    bool check_covered = false;
    check_covered = bg::covered_by(poly, poly);
    if (check_covered) {
         std::cout << "Covered: Yes" << std::endl;
    } else {
        std::cout << "Covered: No" << std::endl;
    }

    bool check_cross = false;
    check_cross = bg::crosses(poly, poly);
    if (check_cross) {
         std::cout << "Cross: Yes" << std::endl;
    } else {
        std::cout << "Cross: No" << std::endl;
    }

    typedef boost::geometry::model::d2::point_xy<float> point_type;
    typedef boost::geometry::model::polygon<point_type> polygon_type;

    polygon_type poly2;
    // boost::geometry::read_wkt(
    //     "POLYGON((0 0,0 10,10 10,10 0,0 0),(1 1,4 1,4 4,1 4,1 1))", poly2);
    boost::geometry::read_wkt(
        "POLYGON((0 0,0 10,10 10,10 0,0 0))", poly2);

    polygon_type res;

    boost::geometry::densify(poly2, res, 6.0);

    std::cout << "densified: " << boost::geometry::wkt(res) << std::endl;

    create_svg("./build/bin/test_densify.svg", poly2, res);

    return 0;
}