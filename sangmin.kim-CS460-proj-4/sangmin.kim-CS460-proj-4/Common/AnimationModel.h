/*
 * Author		: Ryan Kim.
 * Date			: 2022-10-07
 * Description	: Class for animating object.
 */


#pragma once
#include <chrono>
#include <string>
#include <assimp/Importer.hpp>
#include "AnimationStructure.hpp"
#include "AnimationModelDatas.h"

struct aiNode;
class Camera;
class Shader;

typedef Assimp::Importer Importer;

class AnimationModel
{
public:
	enum class TextureInfos
	{
		NONE = 0,
		TEXTURED
	};

	AnimationModel(Shader* shaderVal, std::string _filePath);
	~AnimationModel();
	
	void Select();
	void CheckBuffers();
	void Draw(const glm::mat4& objMat, const glm::mat4& projViewMat,
		float animationT, int transformsOffset, unsigned animationIndex);
	aiNode* GetRootNode();
	const aiScene* GetScene();
	void PopulateTransforms(std::vector<glm::mat4>& transforms);
	
	AnimationModelDatas* datas;
	std::chrono::system_clock::time_point startTime;

	TextureInfos isTextured = TextureInfos::NONE;

private:
	Importer* importer;
	const aiScene* scene;
	const aiScene* check;
	aiNode* rootNode;
	Shader* shader;

	unsigned vao;
	int numVertices, numIndices;
	std::string filePath;

};
