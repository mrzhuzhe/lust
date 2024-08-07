#pragma once

#include "Curve.h"

class Bezier : public Curve
{
public:
	Bezier();
	virtual ~Bezier();

protected:
	virtual void _on_way_point_added();

protected:
	Vector interpolate(double u, const Vector& P0, const Vector& P1, const Vector& P2, const Vector& P3);
};
