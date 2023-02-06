/*
 * Author		: Ryan Kim.
 * Date			: 2022-10-07
 * Description	: Class for populate SSBOs of animating object.
 */


#pragma once
#include <vector>
#include "VertexBoneData.hpp"

class BoneStorageManager
{
public:
	BoneStorageManager(const std::vector<VertexBoneData>& boneInfos);
	~BoneStorageManager();

	std::vector<int> bones;
	std::vector<float> weights;
	std::vector<int> indexStarts;
	std::vector<int> indexEnds;


private:
	

};
