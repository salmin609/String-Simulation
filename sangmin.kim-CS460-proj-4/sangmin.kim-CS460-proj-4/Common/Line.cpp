/*
 * Author		: Ryan Kim.
 * Date			: 2022-11-07
 * Description	: Class for handling path.
 */

#include "Line.h"

#include "ArcLengthTable.h"
#include "CubicSpline.h"

Line::Line()
{
	points.emplace_back(0.f, 0.f, -20.f);
	points.emplace_back(-25.f, 0.f, -10.f);
	points.emplace_back(-32.f, 0.f, 2.5f);
	points.emplace_back(-25.f, 0.f, 10.f);
	points.emplace_back(-3.f, 0.f, 24.f);
	points.emplace_back(20.f, 0.f, 5.f);
	points.emplace_back(50.f, 0.f, 0.f);
	points.emplace_back(25.f, 0.f, -10.f);

	splines.push_back(new CubicSpline(points[0], points[1], points[2], points[3]));
	splines.push_back(new CubicSpline(points[1], points[2], points[3], points[4]));
	splines.push_back(new CubicSpline(points[2], points[3], points[4], points[5]));
	splines.push_back(new CubicSpline(points[3], points[4], points[5], points[6]));
	splines.push_back(new CubicSpline(points[4], points[5], points[6], points[7]));
	splines.push_back(new CubicSpline(points[5], points[6], points[7], points[0]));
	splines.push_back(new CubicSpline(points[6], points[7], points[0], points[1]));
	splines.push_back(new CubicSpline(points[7], points[0], points[1], points[2]));

	const size_t splinesSize = splines.size();

	for(size_t i = 0; i < splinesSize; ++i)
	{
		CubicSpline* spline = splines[i];

		for (float t = 0.f; t < 1.f; t += 0.05f)
		{
			glm::vec3 newCoord = spline->Interpolate(t);

			coords.push_back(newCoord);
		}
	}

	table = new ArcLengthTable();

	//table->SetTable(splines[0]);

	for(size_t i = 0; i < splinesSize; ++i)
	{
		table->SetTable(splines[i]);
	}

	table->FinializeTable();

	checkSplineOffset = 1.f / static_cast<float>(splines.size());
}

Line::~Line()
{
	const size_t splinesSize = splines.size();
	for(size_t i = 0; i < splinesSize; ++i)
	{
		delete splines[i];
	}
}

float Line::GetParam(float dist)
{
	return static_cast<float>(table->GetParamValue(dist));
}

glm::vec3 Line::CheckInterpolation(float param)
{
	return splines[interpolatingSplineIndex]->Interpolate(param);
}

glm::vec3 Line::CheckDerivativeInterpolation(float param)
{
	return splines[interpolatingSplineIndex]->InterpolateDerivative(param);
}

void Line::ChangeSplineIndex()
{
	interpolatingSplineIndex++;

	if (interpolatingSplineIndex >= splines.size())
	{
		interpolatingSplineIndex = 0;
	}
}


