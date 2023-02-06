/*
 * Author		: Ryan Kim.
 * Date			: 2022-11-07
 * Description	: Class for handling path.
 */

#pragma once
#include <vector>
#include "glm/vec3.hpp"

class ArcLengthTable;
class CubicSpline;

class Line
{
public:
	Line();
	~Line();
	const std::vector<glm::vec3>& Coords() const
	{
		return coords;
	}
	float GetParam(float dist);
	glm::vec3 CheckInterpolation(float param);
	glm::vec3 CheckDerivativeInterpolation(float param);
	void ChangeSplineIndex();
	ArcLengthTable* table;

private:
	float checkSplineOffset;
	std::vector<glm::vec3> points;
	std::vector<glm::vec3> coords;

	std::vector<CubicSpline*> splines;

	unsigned interpolatingSplineIndex = 0;

};
