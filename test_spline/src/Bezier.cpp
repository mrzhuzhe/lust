//  https://github.com/chen0040/cpp-spline/tree/master
#include <opencv2/opencv.hpp>
#include "Bezier.h"
#include <iostream>
#include <vector>
Bezier::Bezier()
: Curve()
{

}

Bezier::~Bezier()
{
}

void Bezier::_on_way_point_added()
{
	if(_way_points.size() < 4)
	{
		return;
	}

	int new_control_point_index=_way_points.size() - 1;

	if(new_control_point_index == 3)
	{
		for(int i=0; i<=_steps; i++)
		{
			double u=(double)i / (double)_steps;

			add_node(interpolate(u, _way_points[0], _way_points[1], _way_points[2], _way_points[3]));
		}
	}
	else
	{
		if(new_control_point_index % 2 == 0)
		{
			return;
		}
		
		int pt=new_control_point_index - 2;
		for(int i=0; i<=_steps; i++)
		{
			double u=(double)i / (double)_steps;
			Vector point4=2 * _way_points[pt] - _way_points[pt-1];

			add_node(interpolate(u, _way_points[pt], point4, _way_points[pt+1], _way_points[pt+2]));
		}
	}
}

Vector Bezier::interpolate(double u, const Vector& P0, const Vector& P1, const Vector& P2, const Vector& P3)
{
	Vector point;
	point=u*u*u*((-1) * P0 + 3 * P1 - 3 * P2 + P3);
	point+=u*u*(3*P0 - 6 * P1+ 3 * P2);
	point+=u*((-3) * P0 + 3 * P1);
	point+=P0;

	return point;
}

int main(){
    int rows = 320;
    int cols = rows;
    cv::Mat image(rows, cols, CV_32F);
    
    Curve* curve = new Bezier();
	curve->set_steps(1000); // generate 100 interpolate points between the last 4 way points
    std::vector xlist = {40, 50, 100, 150, 200, 300};
    std::vector ylist = {40, 90, 160, 50, 300, 300};
    cv::Point p0{0, 0};
    for (int i=0; i< xlist.size(); i++) {
        curve->add_way_point(Vector(xlist[i], ylist[i], 0));
        image.at<float>(xlist[i], ylist[i]) = 255;
        p0.y = xlist[i];
        p0.x = ylist[i];
        cv::circle(image, p0, 10, 255);
    }
    
	std::cout << "nodes: " << curve->node_count() << std::endl;
	std::cout << "total length: " << curve->total_length() << std::endl;
	for (int i = 0; i < curve->node_count(); ++i) {
		//std::cout << "node #" << i << ": " << curve->node(i).toString() << " (length so far: " << curve->length_from_starting_point(i) << ")" << std::endl;
        std::cout << " " << i << " " << curve->node(i).x << " " << curve->node(i).x  << std::endl;
        image.at<float>(curve->node(i).x, curve->node(i).y) = 255;
    }

    int key = 0;
    while(key != 27)
    {
        cv::imshow("image", image);
        key = cv::waitKey(10);
    }
    delete curve;
    return 0;
}