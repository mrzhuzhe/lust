// prepare for cgal  https://www.boost.org/doc/libs/1_85_0/libs/geometry/doc/html/geometry/quickstart.html

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>

using namespace boost::geometry;

int main()
{
    model::d3::point_xyz<int> p1(1, 1, 1), p2(2, 2, 2);
    std::cout << "Distance p1-p2 is: " << distance(p1, p2) << std::endl;
    return 0;
}