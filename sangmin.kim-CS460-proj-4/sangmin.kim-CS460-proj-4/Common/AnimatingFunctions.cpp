/*
 * Author		: Ryan Kim.
 * Date			: 2022-10-07
 * Description	: Functions for read / parse / load animating object
 */

#include "AnimatingFunctions.h"

#include <iostream>

#include "AnimationModel.h"
#include <assimp/scene.h>
#include <GL/glew.h>
#include <glm/gtx/transform.hpp>

#include "Interpolation.h"
#include "Material.h"
#include "Texture.h"

#include "Quaternion.h"


namespace AnimatingFunctions
{
	glm::mat4 mat4_cast(const aiMatrix4x4& value)
	{
		glm::mat4 result;
		result[0] = glm::vec4(value[0][0], value[1][0], value[2][0], value[3][0]);
		result[1] = glm::vec4(value[0][1], value[1][1], value[2][1], value[3][1]);
		result[2] = glm::vec4(value[0][2], value[1][2], value[2][2], value[3][2]);
		result[3] = glm::vec4(value[0][3], value[1][3], value[2][3], value[3][3]);

		return result;
	}
	glm::mat4 mat4_cast(const aiMatrix3x3& value)
	{
		glm::mat4 result;
		result[0] = glm::vec4(value[0][0], value[1][0], value[2][0], 0.f);
		result[1] = glm::vec4(value[0][1], value[1][1], value[2][1], 0.f);
		result[2] = glm::vec4(value[0][2], value[1][2], value[2][2], 0.f);
		result[3] = glm::vec4(0.f, 0.f, 0.f, 1.f);

		return result;
	}
	std::vector<std::string> split(std::string s, std::string delimiter) {
		size_t pos_start = 0, pos_end, delim_len = delimiter.length();
		std::string token;
		std::vector<std::string> res;

		while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
			token = s.substr(pos_start, pos_end - pos_start);
			pos_start = pos_end + delim_len;
			res.push_back(token);
		}

		res.push_back(s.substr(pos_start));
		return res;
	}

	

	namespace MeshInitializing
	{
		void InitAllMeshes(AnimationModel* model)
		{
			unsigned size = model->datas->meshes.size();

			for (unsigned i = 0; i < size; ++i)
			{
				aiMesh* pMesh = model->GetScene()->mMeshes[i];
				InitSingleMesh(pMesh, i, model);
			}
		}
		void InitSingleMesh(aiMesh* mesh, unsigned meshIndex, AnimationModel* model)
		{
			const aiVector3D zeroVec(0.0f, 0.0f, 0.0f);

			unsigned size = mesh->mNumVertices;

			for (unsigned i = 0; i < size; ++i)
			{
				const aiVector3D& pos = mesh->mVertices[i];
				model->datas->positions.emplace_back(pos.x, pos.y, pos.z);

				aiVector3D normal, texCoord;

				if (mesh->HasNormals())
					normal = mesh->mNormals[i];
				else
					normal = aiVector3D(0.f, 1.f, 0.f);

				model->datas->normals.emplace_back(normal.x, normal.y, normal.z);

				if (mesh->HasTextureCoords(0))
					texCoord = mesh->mTextureCoords[0][i];
				else
					texCoord = zeroVec;

				model->datas->texCoords.emplace_back(texCoord.x, texCoord.y);
			}

			BoneLoading::LoadMeshBones(mesh, meshIndex, model);

			for (unsigned i = 0; i < mesh->mNumFaces; ++i)
			{
				const aiFace& face = mesh->mFaces[i];

				model->datas->indices.emplace_back(face.mIndices[0]);
				model->datas->indices.emplace_back(face.mIndices[1]);
				model->datas->indices.emplace_back(face.mIndices[2]);
			}
		}

		
	}
	namespace MaterialInitializing
	{
		void InitMaterials(const std::string filename, AnimationModel* model)
		{
			const aiScene* scene = model->GetScene();

			for (unsigned i = 0; i < scene->mNumMaterials; ++i)
			{
				const aiMaterial* mat = scene->mMaterials[i];
				
				LoadTextures(mat, filename, i, model);
				LoadColors(mat, i, model);
			}
		}

		void LoadTextures(const aiMaterial* material, const std::string filename, int index, AnimationModel* model)
		{
			LoadDiffuseTexture(material, filename, index, model);
			LoadSpecularTexture(material, filename, index, model);
		}

		void LoadDiffuseTexture(const aiMaterial* material, const std::string filename, int index, AnimationModel* model)
		{
			model->datas->materials[index].pDiffuse = nullptr;

			const unsigned textureCount = material->GetTextureCount(aiTextureType_DIFFUSE);

			if(textureCount > 0)
			{
				aiString path;

				if(material->GetTexture(aiTextureType_DIFFUSE, 0, &path, nullptr, nullptr, nullptr, nullptr, nullptr) == AI_SUCCESS)
				{
					std::string p(path.data);

					std::string delimiter = "\\";
					std::vector<std::string> vec = split(p, delimiter);

					/*if (p.substr(0, 2) == ".\\") {
						p = p.substr(2, p.size() - 2);
					}*/

					std::string fullPath = filename + "/" + p;
					std::string filePath = "../Models/" + vec[vec.size() - 1];
					model->datas->materials[index].pDiffuse = new Texture(GL_TEXTURE_2D, filePath);

					if(!model->datas->materials[index].pDiffuse->Load())
					{
						std::cout << "Failed to Load diffuse texture" << std::endl;
						model->isTextured = AnimationModel::TextureInfos::NONE;
						//exit(0);
					}
					else
					{
						std::cout << "Load diffuse texture : " << fullPath << std::endl;
						model->isTextured = AnimationModel::TextureInfos::TEXTURED;
					}
				}
			}
			else
			{
				std::cout << "No texture" << std::endl;
				model->isTextured = AnimationModel::TextureInfos::NONE;

			}

		}
		void LoadSpecularTexture(const aiMaterial* material, const std::string filename, int index, AnimationModel* model)
		{
			model->datas->materials[index].pSpecular = nullptr;

			if(material->GetTextureCount(aiTextureType_SHININESS) > 0)
			{
				aiString path;

				if(material->GetTexture(aiTextureType_SHININESS, 0, &path, nullptr, nullptr, nullptr, nullptr, nullptr) == AI_SUCCESS)
				{
					std::string p(path.data);

					std::string delimiter = "\\";
					std::vector<std::string> vec = split(p, delimiter);

					std::string fullPath = filename + "/" + p;

					model->datas->materials[index].pSpecular = new Texture(GL_TEXTURE_2D, vec[vec.size() - 1]);

					if (!model->datas->materials[index].pSpecular->Load())
					{
						std::cout << "Failed to Load Specular texture" << std::endl;
						//exit(0);
					}
					else
						std::cout << "Load specular texture : " << fullPath << std::endl;
				}
			}
		}

		void LoadColors(const aiMaterial* material, int index, AnimationModel* model)
		{
			aiColor3D ambientColor(0.f, 0.f, 0.f);
			glm::vec3 oneVec(1.f, 1.f, 1.f);

			int shadingModel = 0;

			if (material->Get(AI_MATKEY_SHADING_MODEL, shadingModel) == AI_SUCCESS)
				std::cout << "Shading Model : " << shadingModel << std::endl;

			if (material->Get(AI_MATKEY_COLOR_AMBIENT, ambientColor) == AI_SUCCESS)
			{
				model->datas->materials[index].ambientColor.r = ambientColor.r;
				model->datas->materials[index].ambientColor.g = ambientColor.g;
				model->datas->materials[index].ambientColor.b = ambientColor.b;
			}
			else
				model->datas->materials[index].ambientColor = oneVec;

			aiColor3D diffuseColor(0.f, 0.f, 0.f);

			if(material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor) == AI_SUCCESS)
			{
				model->datas->materials[index].diffuseColor.r = diffuseColor.r;
				model->datas->materials[index].diffuseColor.g = diffuseColor.g;
				model->datas->materials[index].diffuseColor.b = diffuseColor.b;
			}

			aiColor3D specularColor(0.f, 0.f, 0.f);

			if(material->Get(AI_MATKEY_COLOR_SPECULAR, specularColor) == AI_SUCCESS)
			{
				model->datas->materials[index].specularColor.r = specularColor.r;
				model->datas->materials[index].specularColor.g = specularColor.g;
				model->datas->materials[index].specularColor.b = specularColor.b;
			}
			
		}
	}
	namespace BoneLoading
	{
		void LoadMeshBones(aiMesh* mesh, unsigned meshIndex, AnimationModel* model)
		{
			const unsigned size = mesh->mNumBones;

			for (unsigned i = 0; i < size; ++i)
				LoadSingleBone(mesh->mBones[i], meshIndex, model);
		}

		void LoadSingleBone(aiBone* bone, unsigned meshIndex, AnimationModel* model)
		{
			const int boneId = GetBoneID(bone, model);

			if (boneId == model->datas->boneInfos.size())
			{
				glm::mat4 mat = mat4_cast(bone->mOffsetMatrix);
				BoneInfo boneInfo(mat);
				model->datas->boneInfos.push_back(boneInfo);
			}

			for (unsigned i = 0; i < bone->mNumWeights; ++i)
			{
				aiVertexWeight& vw = bone->mWeights[i];
				unsigned globalVertexId = model->datas->meshes[meshIndex].BaseVertex + bone->mWeights[i].mVertexId;

				model->datas->bones[globalVertexId].AddBoneData(boneId, vw.mWeight);
			}
		}

		int GetBoneID(const aiBone* bone, AnimationModel* model)
		{
			int boneIndex;
			std::string name(bone->mName.C_Str());

			//means not found
			if (model->datas->boneName2IndexMap.find(name) == model->datas->boneName2IndexMap.end())
			{
				boneIndex = static_cast<int>(model->datas->boneName2IndexMap.size());
				model->datas->boneName2IndexMap[name] = boneIndex;
			}
			else
				boneIndex = model->datas->boneName2IndexMap[name];

			return boneIndex;
		}
	}
	namespace AnimationMatrix
	{
		void GetBoneTransforms(std::vector<glm::mat4>& transforms, float timeInSeconds, const aiScene* scene, AnimationModel* model, unsigned animationIndex)
		{
			transforms.resize(model->datas->boneInfos.size());

			const aiAnimation* animation = scene->mAnimations[animationIndex];
			const glm::mat4 identityMat = glm::mat4(1.f);

			const float ticksPerSecond = static_cast<float>(animation->mTicksPerSecond != 0 ? animation->mTicksPerSecond : 25.f);
			const float timeInTicks = timeInSeconds * ticksPerSecond;
			const float animationTimeTicks = fmod(timeInTicks, static_cast<float>(animation->mDuration));

			ReadNodeHierarchy(scene->mRootNode, identityMat, animationTimeTicks, scene, model, animationIndex);

			const uint size = model->datas->boneInfos.size();

			for (uint i = 0; i < size; ++i)
				transforms[i] = model->datas->boneInfos[i].finalTransform;
		}

		void ReadNodeHierarchy(const aiNode* node, const glm::mat4& parentTransform, float animationTimeTicks, const aiScene* scene, AnimationModel* model, int animationIndex)
		{
			std::string nodeName(node->mName.data);

			const aiAnimation* pAnimation = scene->mAnimations[animationIndex];
			const aiNodeAnim* pNodeAnim = FindNodeAnimation(pAnimation, nodeName);

			glm::mat4 nodeTransform(AnimatingFunctions::mat4_cast(node->mTransformation));


			if (pNodeAnim)
			{
				aiVector3D scaling;
				Interpolation::CalcInterpolatingScaling(scaling, animationTimeTicks, pNodeAnim);
				glm::mat4 scalingM;
				scalingM = glm::scale(scalingM, glm::vec3(scaling.x, scaling.y, scaling.z));

				Quaternion rotationQ;
				Interpolation::CalcInterpolatedRotation(rotationQ, animationTimeTicks, pNodeAnim);
				const glm::mat4 rotationM = rotationQ.GetMatrix();

				aiVector3D translation;
				Interpolation::CalcInterpolatedPosition(translation, animationTimeTicks, pNodeAnim);
				glm::mat4 translationM;
				translationM = glm::translate(translationM, glm::vec3(translation.x, translation.y, translation.z));

				nodeTransform = translationM * rotationM * scalingM;
			}

			const glm::mat4 globalTransform = parentTransform * nodeTransform;

			//Means find
			if (model->datas->boneName2IndexMap.find(nodeName) != model->datas->boneName2IndexMap.end())
			{
				const uint boneIndex = model->datas->boneName2IndexMap[nodeName];
				model->datas->boneInfos[boneIndex].finalTransform = globalTransform * model->datas->boneInfos[boneIndex].offsetMat;
			}

			for (uint i = 0; i < node->mNumChildren; ++i)
				ReadNodeHierarchy(node->mChildren[i], globalTransform, animationTimeTicks, scene, model, animationIndex);
		}

		aiNodeAnim* FindNodeAnimation(const aiAnimation* pAnimation, const std::string& nodeName)
		{
			for (uint i = 0; i < pAnimation->mNumChannels; ++i)
			{
				aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];

				if (std::string(pNodeAnim->mNodeName.data) == nodeName)
					return pNodeAnim;
			}

			return NULL;
		}
	}

}




