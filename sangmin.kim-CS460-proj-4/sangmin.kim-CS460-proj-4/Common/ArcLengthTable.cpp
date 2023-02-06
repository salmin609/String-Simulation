/*
 * Author		: Ryan Kim.
 * Date			: 2022-11-07
 * Description	: Functions for build arc length table.
 */

#include "ArcLengthTable.h"

#include <glm/detail/func_geometric.inl>

#include "CubicSpline.h"

ArcLengthTable::ArcLengthTable()
= default;

ArcLengthTable::~ArcLengthTable()
= default;

void ArcLengthTable::SetTable(CubicSpline* spline)
{
	std::vector<ArcLengthTableValue> tableValue;

	if(forTheFirstTime)
	{
		tableValue.push_back(ArcLengthTableValue{ startParam, 0.0 });
		startParam += distanceBtwEntries;
		forTheFirstTime = false;
	}

	for(double t = startParam; t <= startParam + 1.0; t += distanceBtwEntries)
	{
		glm::vec3 pos1 = spline->Interpolate(t - 0.005);
		glm::vec3 pos2 = spline->Interpolate(t);

		const double distance = static_cast<double>(glm::distance(pos1, pos2));

		ArcLengthTableValue val{};

		val.parametric = t;
		val.arcLength = distance + prevArcLength;


		tableValue.push_back(val);

		prevArcLength = val.arcLength;
	}

	table.push_back(tableValue);

	startParam += 1.0;
}

void ArcLengthTable::FinializeTable()
{
	const size_t tablesCount = table.size();
	double maximumArcLength = 0.0;

	for(size_t i = 0; i < tablesCount; ++i)
	{
		std::vector<ArcLengthTableValue> table_ = table[i];

		const size_t tableSize = table_.size();

		for(size_t j = 0; j < tableSize; ++j)
		{
			ArcLengthTableValue val = table_[j];

			if (val.arcLength > maximumArcLength)
				maximumArcLength = val.arcLength;

			finalTable.push_back(val);
		}
	}

	const float maxN = static_cast<float>(tablesCount);

	const size_t totalSize = finalTable.size();

	for(size_t i = 0; i < totalSize; ++i)
	{
		finalTable[i].parametric /= maxN;
		finalTable[i].arcLength /= maximumArcLength;
	}

	distanceBtwEntries = finalTable[1].parametric;

}

int ArcLengthTable::GetClosestIndex(double u)
{
	const double uDivDu = u / distanceBtwEntries;
	int result = static_cast<int>(uDivDu);

	if (result >= static_cast<int>(finalTable.size()))
		result = static_cast<int>(finalTable.size()) - 1;

	return result;
}

double ArcLengthTable::GetArcLength(double u)
{
	double du = distanceBtwEntries;

	const int index = GetClosestIndex(u);

	double ui = finalTable[index].parametric;

	double k = (u - ui) / du;

	double si = finalTable[index].arcLength;
	double nextSi = finalTable[index + 1].arcLength;

	double s = si + k * (nextSi - si);

	return s;
}

double ArcLengthTable::GetParamValue(double s)
{
	//bin search
	int closestIndex;
	
	double uMin = 0.0;
	double uMax = 1.0;

	double diff;
	double midDiff;
	int loopCount = 0;

	do
	{
		const double uMiddle = (uMin + uMax) / 2.0;
		closestIndex = GetClosestIndex(uMiddle);

		const int prevIndex = closestIndex - 1;

		const double distanceValue = finalTable[closestIndex].arcLength;

		midDiff =
			abs(finalTable[closestIndex].arcLength - finalTable[prevIndex].arcLength);

		diff = abs(distanceValue - s);

		if(distanceValue > s)
			uMax = uMiddle;
		else
			uMin = uMiddle;

		loopCount++;

		if (loopCount > 50)
			break;
	} while (diff > midDiff);
	
	return finalTable[closestIndex].parametric;
}
