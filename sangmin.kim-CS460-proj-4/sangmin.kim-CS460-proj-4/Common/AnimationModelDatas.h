/*
 * Author		: Ryan Kim.
 * Date			: 2022-10-07
 * Description	: Class for storing all informations of animating object.
 */


#pragma once

#include <map>
#include <string>
#include <vector>
#include "Material.h"
#include "VertexBoneData.hpp"
#include <glm/detail/type_vec.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "AnimationStructure.hpp"

class BoneStorageManager;
struct aiScene;
class Buffer;

class AnimationModelDatas
{
public:
	AnimationModelDatas();
	~AnimationModelDatas();
	void ReserveVectorSpace();
	void ReserveSpace(const aiScene* scene);
	void PopulateBuffers(unsigned vao);
	void PopulateTransforms(unsigned vao, std::vector<glm::mat4>& transforms);

	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texCoords;
	std::vector<unsigned> indices;

	std::vector<BoneInfo> boneInfos;
	std::vector<VertexBoneData> bones;

	std::map<std::string, uint> boneName2IndexMap;
	std::vector<BasicMeshEntry> meshes;
	std::vector<Material> materials;

	Buffer* posBuffer;
	Buffer* texBuffer;
	Buffer* normalBuffer;
	Buffer* indexBuffer;

	int numVertices, numIndices;
	unsigned ssboBones, ssboWeights, ssboIndexStarts, ssboIndexEnds, ssboTransforms;
	BoneStorageManager* storage;

	void PopulateShaderStorage();
private:

};
