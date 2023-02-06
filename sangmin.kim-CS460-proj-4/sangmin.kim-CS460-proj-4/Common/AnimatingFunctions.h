/*
 * Author		: Ryan Kim.
 * Date			: 2022-10-07
 * Description	: Functions for read / parse / load animating object
 */

#pragma once

#include <string>
#include <vector>
#include <assimp/mesh.h>
#include "AnimationStructure.hpp"

struct aiBone;
class Material;
struct aiMaterial;
struct aiNodeAnim;
struct aiAnimation;
struct aiNode;
struct aiScene;
struct AnimationModel;
struct Texture;

namespace AnimatingFunctions
{
	glm::mat4 mat4_cast(const aiMatrix4x4& value);
	glm::mat4 mat4_cast(const aiMatrix3x3& value);

	namespace MeshInitializing
	{
		void InitAllMeshes(AnimationModel* model);
		void InitSingleMesh(aiMesh* mesh, unsigned meshIndex, AnimationModel* model);
	}
	namespace MaterialInitializing
	{
		void InitMaterials(const std::string filename, AnimationModel* model);
		void LoadTextures(const aiMaterial* material, const std::string filename, int index, AnimationModel* model);
		void LoadDiffuseTexture(const aiMaterial* material, const std::string filename, int index, AnimationModel* model);
		void LoadSpecularTexture(const aiMaterial* material, const std::string filename, int index, AnimationModel* model);
		void LoadColors(const aiMaterial* material, int index, AnimationModel* model);
	}
	namespace BoneLoading
	{
		void LoadMeshBones(aiMesh* mesh, unsigned meshIndex, AnimationModel* model);
		void LoadSingleBone(aiBone* bone, unsigned meshIndex, AnimationModel* model);
		int GetBoneID(const aiBone* bone, AnimationModel* model);
	}
	namespace AnimationMatrix
	{
		void GetBoneTransforms(std::vector<glm::mat4>& transforms, float timeInSeconds, const aiScene* scene, AnimationModel* model, unsigned animationIndex);
		void ReadNodeHierarchy(const aiNode* node, const glm::mat4& parentTransform, float animationTimeTicks, const aiScene* scene, AnimationModel* model, int animationIndex);
		aiNodeAnim* FindNodeAnimation(const aiAnimation* pAnimation, const std::string& nodeName);
	}
}
