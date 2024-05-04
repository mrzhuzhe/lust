// prepare for cgal  https://www.boost.org/doc/libs/1_85_0/libs/geometry/doc/html/geometry/quickstart.html

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>

// #include <boost/geometry/geometries/adapted/c_array.hpp>

// BOOST_GEOMETRY_REGISTER_C_ARRAY_CS(cs::cartesian)

// #include <boost/geometry/geometries/adapted/boost_tuple.hpp>

// BOOST_GEOMETRY_REGISTER_BOOST_TUPLE_CS(cs::cartesian)

using namespace boost::geometry;
typedef boost::geometry::model::point<float, 2, boost::geometry::cs::spherical_equatorial<boost::geometry::degree>> spherical_point;

int main()
{
    model::d3::point_xyz<float> p1(1, 1, 1), p2(2, 2, 2);
    std::cout << "Distance p1-p2 is: " << distance(p1, p2) << std::endl;

    model::polygon<model::d2::point_xy<float> > poly;
    //float points[][2] = {{1,2}, {3, 4}, {5, 6}, {7, 8}}; 
    
    model::d2::point_xy<float> points[] = {{-1 , 0}, {0, 1}, {1, 0}, {0.5, -0.5}, {-0.5, -0.5}}; 
    append(poly, points);

    model::d2::point_xy<float> p{0, 0};

    std::cout << "Point p is in polygon? " << std::boolalpha << within(p, poly) << std::endl;

    std::cout << "Area: " << area(poly) << std::endl;

    // non-catercian 
    spherical_point amsterdam(4.90, 52.37);
    spherical_point paris(2.35, 48.86);

    float const earth_radius = 3959; // miles
    std::cout << "Distance in miles: " << distance(amsterdam, paris) * earth_radius << std::endl;

    return 0;
}