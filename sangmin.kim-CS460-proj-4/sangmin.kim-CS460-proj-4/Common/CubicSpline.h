/*
 * Author		: Ryan Kim.
 * Date			: 2022-11-07
 * Description	: Functions for build cubic spline.
 */

#pragma once
#include "glm/vec3.hpp"

class CubicSpline
{
public:
	CubicSpline(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);
	~CubicSpline();
	glm::vec3 Interpolate(float t);
	glm::vec3 InterpolateDerivative(float t);
private:
	glm::vec3 a, b, c, d;
};

inline CubicSpline::CubicSpline(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
{
	a = -(0.5f * p0) + ((3.f / 2.f) * p1) - ((3.f / 2.f) * p2) + ((1.f / 2.f) * p3);
	b = p0 - ((5.f / 2.f) * p1) + (2.f * p2) - ((1.f / 2.f) * p3);
	c = -((1.f / 2.f) * p0) + ((1.f / 2.f) * p2);
	d = p1;
}

inline CubicSpline::~CubicSpline()
{
}

inline glm::vec3 CubicSpline::Interpolate(float t)
{
	glm::vec3 result = (a * (t * t * t)) + (b * (t * t)) + (c * (t)) + d;
	return result;
}

inline glm::vec3 CubicSpline::InterpolateDerivative(float t)
{
	glm::vec3 result = (3.f * a * (t * t)) + (2.f * b * (t)) + c;
	return result;
}
