#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Delaunay_triangulation_3.h>
#include <CGAL/point_generators_3.h>
#include <CGAL/draw_triangulation_3.h>
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_3<K>                   DT3;
typedef CGAL::Creator_uniform_3<double,K::Point_3>          Creator;

typedef CGAL::Delaunay_triangulation_2<K>                   DT2;

#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <cassert>
#include <iostream>
typedef CGAL::Exact_predicates_tag                               Itag;
typedef CGAL::Constrained_Delaunay_triangulation_2<K, CGAL::Default, Itag> CDT;
typedef CDT::Point Point;
typedef CDT::Edge  Edge;
#include <CGAL/draw_triangulation_2.h>

#include <CGAL/Polygon_2.h>

#include <CGAL/draw_polygon_2.h>

typedef CGAL::Polygon_2<K>                                  Polygon_2;
typedef CGAL::Point_2<K>                                    P2;

int main()
{
  // std::vector<K::Point_3> points;
  // CGAL::Random_points_in_sphere_3<K::Point_3,Creator> g(1.0);
  // std::copy_n(g, 50, std::back_inserter(points));
  // DT3 dt3(points.begin(), points.end());
  // CGAL::draw(dt3);

  Polygon_2 points;
  CDT cdt;
  std::cout << "Inserting a grid of 5x5 constraints " << std::endl;
  for (int i = 1; i < 6; ++i){
    cdt.insert_constraint( Point(0,i), Point(6,i));
    points.push_back(P2(0,i));
    points.push_back(P2(6,i));
  }    
  for (int j = 1; j < 6; ++j){    
    cdt.insert_constraint( Point(j,0), Point(j,6));
    points.push_back(P2(j,0));
    points.push_back(P2(j,6));
  }
  assert(cdt.is_valid());
  int count = 0;
  int count2 = 0;
  for (const Edge& e : cdt.finite_edges()){
    ++count2;
    if (cdt.is_constrained(e))
      ++count;
  }
    
  std::cout << "The number of resulting constrained edges is  ";
  std::cout <<  count << " " << count2 << std::endl;

  //std::cout <<  cdt.size() << std::endl;
  //DT2 dt2(points.begin(), points.end());
  //CGAL::draw(cdt);
  CGAL::draw(dt2);
  CGAL::draw(points);
  return EXIT_SUCCESS;
}