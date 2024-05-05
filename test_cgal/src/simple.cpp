#include <iostream>
// #include <CGAL/Simple_cartesian.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/convex_hull_2.h>

// typedef CGAL::Simple_cartesian<float> Kernel;
// typedef Kernel::Point_2 Point_2;
// typedef Kernel::Segment_2 Segment_2;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point_2;
typedef std::vector<Point_2> Points;
typedef K::Segment_2 Segment_2;

// typedef CGAL::Exact_predicates_inexact_constructions_kernel K3;
// typedef CGAL::Projection_traits_yz_3<K3> K;
// typedef K::Point_2 Point_2;

int main()
{
  Point_2 p(1,1), q(10,10);
  std::cout << "p = " << p << std::endl;
  std::cout << "q = " << q.x() << " " << q.y() << std::endl;
  std::cout << "sqdist(p,q) = "
            << CGAL::squared_distance(p,q) << std::endl;
  Segment_2 s(p,q);
  Point_2 m(5, 9);
  std::cout << "m = " << m << std::endl;
  std::cout << "sqdist(Segment_2(p,q), m) = "
            << CGAL::squared_distance(s,m) << std::endl;
  std::cout << "p, q, and m ";
  switch (CGAL::orientation(p,q,m)){
  case CGAL::COLLINEAR:
    std::cout << "are collinear\n";
    break;
  case CGAL::LEFT_TURN:
    std::cout << "make a left turn\n";
    break;
  case CGAL::RIGHT_TURN:
    std::cout << "make a right turn\n";
    break;
  }
  std::cout << " midpoint(p,q) = " << CGAL::midpoint(p,q) << std::endl;

  {
    Point_2 p(0, 0.3), q(1, 0.6), r(2, 0.9);
    std::cout << (CGAL::collinear(p,q,r) ? "collinear\n" : "not collinear\n");
  }
  {
    Point_2 p(0, 1.0/3.0), q(1, 2.0/3.0), r(2, 1);
    std::cout << (CGAL::collinear(p,q,r) ? "collinear\n" : "not collinear\n");
  }
  {
    Point_2 p(0,0), q(1, 1), r(2, 2);
    std::cout << (CGAL::collinear(p,q,r) ? "collinear\n" : "not collinear\n");
  }

  Point_2 p2(0, 0.3), q2, r2(2, 0.9);
  {
    q2  = Point_2(1, 0.6);
    std::cout << (CGAL::collinear(p2,q2,r2) ? "collinear\n" : "not collinear\n");
  }
  {
    std::istringstream input("0 0.3   1 0.6   2 0.9");
    input >> p2 >> q2 >> r2;
    std::cout << (CGAL::collinear(p,q2,r2) ? "collinear\n" : "not collinear\n");
  }
  {
    q2 = CGAL::midpoint(p2,r2);
    std::cout << (CGAL::collinear(p2,q2,r2) ? "collinear\n" : "not collinear\n");
  }

  Points points, result;
  points.push_back(Point_2(0,0));
  points.push_back(Point_2(10,0));
  points.push_back(Point_2(10,10));
  points.push_back(Point_2(6,5));
  points.push_back(Point_2(4,1));
  CGAL::convex_hull_2( points.begin(), points.end(), std::back_inserter(result) );
  std::cout << result.size() << " points on the convex hull" << std::endl;

  return 0;
}