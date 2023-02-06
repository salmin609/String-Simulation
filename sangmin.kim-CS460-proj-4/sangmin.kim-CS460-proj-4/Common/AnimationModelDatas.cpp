/*
 * Author		: Ryan Kim.
 * Date			: 2022-10-07
 * Description	: Class for storing all informations of animating object.
 */


#include "AnimationModelDatas.h"

#include <assimp/scene.h>
#include <GL/glew.h>
#include "Buffer.hpp"

#include "BoneStorageManager.h"

AnimationModelDatas::AnimationModelDatas()
{
	glGenBuffers(1, &ssboBones);
	glGenBuffers(1, &ssboWeights);
	glGenBuffers(1, &ssboIndexStarts);
	glGenBuffers(1, &ssboIndexEnds);
	glGenBuffers(1, &ssboTransforms);
}

AnimationModelDatas::~AnimationModelDatas()
{
	delete posBuffer;
	delete texBuffer;
	delete normalBuffer;
	delete indexBuffer;

	delete storage;

	glDeleteBuffers(1, &ssboBones);
	glDeleteBuffers(1, &ssboWeights);
	glDeleteBuffers(1, &ssboIndexStarts);
	glDeleteBuffers(1, &ssboIndexEnds);
	glDeleteBuffers(1, &ssboTransforms);
}

void AnimationModelDatas::ReserveVectorSpace()
{
	positions.reserve(numVertices);
	normals.reserve(numVertices);
	texCoords.reserve(numVertices);
	indices.reserve(numIndices);
	bones.resize(numVertices);
}

void AnimationModelDatas::ReserveSpace(const aiScene* scene)
{
	meshes.resize(scene->mNumMeshes);
	materials.resize(scene->mNumMaterials);

	const unsigned size = meshes.size();

	for (unsigned i = 0; i < size; ++i)
	{
		BasicMeshEntry& mesh = meshes[i];

		mesh.MaterialIndex = scene->mMeshes[i]->mMaterialIndex;
		mesh.NumIndices = scene->mMeshes[i]->mNumFaces * 3;
		mesh.BaseVertex = numVertices;
		mesh.BaseIndex = numIndices;

		numVertices += scene->mMeshes[i]->mNumVertices;
		numIndices += meshes[i].NumIndices;
	}
	ReserveVectorSpace();
}

void AnimationModelDatas::PopulateBuffers(unsigned vao)
{
	glBindVertexArray(vao);

	posBuffer = new Buffer(GL_ARRAY_BUFFER, sizeof(glm::vec3) * positions.size(), GL_STATIC_DRAW,
		positions.data());
	posBuffer->Bind();
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);
	

	texBuffer = new Buffer(GL_ARRAY_BUFFER, sizeof(glm::vec2) * texCoords.size(), GL_STATIC_DRAW,
		texCoords.data());
	texBuffer->Bind();
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (GLvoid*)0);

	normalBuffer = new Buffer(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), GL_STATIC_DRAW,
		normals.data());
	normalBuffer->Bind();
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);

	storage = new BoneStorageManager(bones);

	PopulateShaderStorage();

	indexBuffer = new Buffer(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * indices.size(),
		GL_STATIC_DRAW, indices.data());

	glBindVertexArray(0);
}

void AnimationModelDatas::PopulateTransforms(unsigned vao, std::vector<glm::mat4>& transforms)
{
	const size_t transformsSize = transforms.size();

	glBindVertexArray(vao);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboTransforms);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::mat4) * transformsSize,
		transforms.data(), GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, ssboTransforms);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	glBindVertexArray(0);
}

void AnimationModelDatas::PopulateShaderStorage()
{
	const size_t bonesSize = storage->bones.size();
	const size_t weightsSize = storage->weights.size();
	const size_t startIndexSize = storage->indexStarts.size();
	const size_t endIndexSize = storage->indexEnds.size();

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboBones);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(int) * bonesSize,
		storage->bones.data(), GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssboBones);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboWeights);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float) * weightsSize,
		storage->weights.data(), GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, ssboWeights);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboIndexStarts);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(int) * startIndexSize,
		storage->indexStarts.data(), GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, ssboIndexStarts);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboIndexEnds);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(int) * endIndexSize,
		storage->indexEnds.data(), GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, ssboIndexEnds);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}
