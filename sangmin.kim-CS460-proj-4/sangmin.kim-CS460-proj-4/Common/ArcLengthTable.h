/*
 * Author		: Ryan Kim.
 * Date			: 2022-11-07
 * Description	: Functions for build arc length table.
 */

#pragma once
#include <vector>

class CubicSpline;

struct ArcLengthTableValue
{
	double parametric;
	double arcLength;
};

class ArcLengthTable
{
public:
	ArcLengthTable();
	~ArcLengthTable();

	std::vector<std::vector<ArcLengthTableValue>> table;
	std::vector<ArcLengthTableValue> finalTable;

	void SetTable(CubicSpline* spline);
	void FinializeTable();

	int GetClosestIndex(double u);
	double GetArcLength(double u);
	double GetParamValue(double s);
	double distanceBtwEntries = 0.005;


private:
	bool forTheFirstTime = true;

	double startParam = 0.0;
	double prevArcLength = 0.0;
};