// https://www.boost.org/doc/libs/1_85_0/libs/geometry/doc/html/geometry/spatial_indexes/rtree_quickstart.html

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/geometries/geometries.hpp>

#include <boost/geometry/index/rtree.hpp>

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
   
    // create some values
    for ( unsigned i = 0 ; i < 10 ; ++i )
    {
        // create a box
        box b(point(i + 0.0f, i + 0.0f), point(i + 0.5f, i + 0.5f));
        // insert new value
        rtree.insert(std::make_pair(b, i));
    }

    // find values intersecting some area defined by a box
    box query_box(point(0, 0), point(5, 5));
    std::vector<value> result_s;
    rtree.query(bgi::intersects(query_box), std::back_inserter(result_s));

    std::cout << "result_s.size(): "<< result_s.size() << std::endl;
    
    // display results
    std::cout << "spatial query box:" << std::endl;
    std::cout << bg::wkt<box>(query_box) << std::endl;
    std::cout << "spatial query result:" << std::endl;
    BOOST_FOREACH(value const& v, result_s)
        std::cout << bg::wkt<box>(v.first) << " - " << v.second << std::endl;

    // find 5 nearest values to a point
    std::vector<value> result_n;
    rtree.query(bgi::nearest(point(0, 0), 5), std::back_inserter(result_n));

    std::cout << "knn query point:" << std::endl;
    std::cout << bg::wkt<point>(point(0, 0)) << std::endl;
    std::cout << "knn query result:" << std::endl;
    BOOST_FOREACH(value const& v, result_n)
    std::cout << bg::wkt<box>(v.first) << " - " << v.second << std::endl;

    bg::model::ring<bg::model::d2::point_xy<float> > ring;


    ring += 
        bg::model::d2::point_xy<float>(4.0, -0.5), bg::model::d2::point_xy<float>(3.5, 1.0),
        bg::model::d2::point_xy<float>(2.0, 1.5), bg::model::d2::point_xy<float>(3.5, 2.0),
        bg::model::d2::point_xy<float>(4.0, 3.5), bg::model::d2::point_xy<float>(4.5, 2.0),
        bg::model::d2::point_xy<float>(6.0, 1.5), bg::model::d2::point_xy<float>(4.5, 1.0),
        bg::model::d2::point_xy<float>(4.0, -0.5);
    // bg::append(ring, point(4.0, -0.5));
    // bg::append(ring, point(3.5, 1.0));
    // bg::append(ring, point(2.0, 1.5));
    // bg::append(ring, point(3.5, 2.0));
    // bg::append(ring, point(4.0, 3.5)); 
    // bg::append(ring, point(4.5, 2.0));
    // bg::append(ring, point(6.0, 1.5)); 
    // bg::append(ring, point(4.5, 1.0));
    // bg::append(ring, point(4.0, -0.5));

    create_svg("./build/bin/test.svg", ring, result_s[1].first);

    return 0;
}