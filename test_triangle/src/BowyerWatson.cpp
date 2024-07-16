#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <array>
#include <random>
#include <chrono>

#include <opencv2/opencv.hpp>

#include "vector2.h"
#include "triangle.h"
#include "delaunay.h"

int main(int argc, char * argv[])
{
	int numberPoints = 40;
	if (argc>1)
	{
		numberPoints = atoi(argv[1]);
	}

	std::default_random_engine eng(std::random_device{}());
	std::uniform_real_distribution<double> dist_w(0, 800);
	std::uniform_real_distribution<double> dist_h(0, 600);

	std::cout << "Generating " << numberPoints << " random points" << std::endl;

	std::vector<dt::Vector2<double>> points;
	for(int i = 0; i < numberPoints; ++i) {
		points.push_back(dt::Vector2<double>{dist_w(eng), dist_h(eng)});
	}

	dt::Delaunay<double> triangulation;
	const auto start = std::chrono::high_resolution_clock::now();
	const std::vector<dt::Triangle<double>> triangles = triangulation.triangulate(points);
	const auto end = std::chrono::high_resolution_clock::now();
	const std::chrono::duration<double> diff = end - start;

	std::cout << triangles.size() << " triangles generated in " << diff.count()
			<< "s\n";
	const std::vector<dt::Edge<double>> edges = triangulation.getEdges();

	int rows = 800;
    int cols = 600;
    cv::Mat image(cols, rows, CV_32F);
	cv::Point p1(0, 0), p2(0, 0);
	for(const auto &e : edges) {
		p1.x = e.v->x;
		p1.y = e.v->y;
		p2.x = e.w->x;
		p2.y = e.w->y;
		cv::line(image, p1, p2, 2);
	}

	int key = 0;
    while(key != 27)
    {
        cv::imshow("image", image);
        key = cv::waitKey(10);
    }

	// // Transform each points of each vector as a rectangle
	// for(const auto p : points) {
	// 	sf::RectangleShape s{sf::Vector2f(4, 4)};
	// 	s.setPosition(static_cast<float>(p.x), static_cast<float>(p.y));
	// 	window.draw(s);
	// }

	return 0;
}