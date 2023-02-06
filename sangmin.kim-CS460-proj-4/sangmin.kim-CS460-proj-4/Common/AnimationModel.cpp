/*
 * Author		: Ryan Kim.
 * Date			: 2022-10-07
 * Description	: Class for animating object.
 */


#include "AnimationModel.h"

#include <assimp/scene.h>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

#include "AnimatingFunctions.h"
#include "Camera.hpp"
#include "Shader.h"
#include "assimp/postprocess.h"

#include "Texture.h"
#include "AnimationModelDatas.h"

#define ASSIMP_LOAD_FLAGS (aiProcess_Triangulate | aiProcess_GenSmoothNormals |  aiProcess_JoinIdenticalVertices )

/*
 * Create VAO for object,
 *
 * Create Assimp::Importer to read / Parse files.
 *
 * AnimationModelDatas
 * Store all infos about model (positions, normals, textureCoords, index)
 * Also, I use Shader Storage Buffer Object to store boneIndexes, weights, BoneStartIndex, BoneEndIndex, so it stores index of ssbo.
 *
 * After that, Reserve VectorSpace,
 * Read positions / texCoords / normals hierarchical, Also, Find & load texture files.
 */
AnimationModel::AnimationModel(Shader* shaderVal, std::string _filePath)
{
	assert(shaderVal != nullptr);
	shader = shaderVal;

	numVertices = 0;
	numIndices = 0;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(0);
	
	startTime = std::chrono::system_clock::now();
	filePath = _filePath;
	importer = new Importer();
	
	scene = importer->ReadFile(filePath.c_str(),
		ASSIMP_LOAD_FLAGS);

	datas = new AnimationModelDatas();

	datas->ReserveSpace(scene);
	AnimatingFunctions::MeshInitializing::InitAllMeshes(this);
	datas->PopulateBuffers(vao);
	AnimatingFunctions::MaterialInitializing::InitMaterials(filePath, this);
}

AnimationModel::~AnimationModel()
{
	delete importer;
	delete datas;
}


void AnimationModel::Select()
{
	glBindVertexArray(vao);
}

void AnimationModel::CheckBuffers()
{
}

/*
 * Store all animation transformation datas in transforms vector,
 * read hierarchical transformation from all nodes, 
 * and pass infos to shader uniform.
 *
 */
void AnimationModel::Draw(
	const glm::mat4& objMat, const glm::mat4& projViewMat, float animationT, int transformsOffset,
	unsigned animationIndex)
{
	assert(shader != nullptr);

	shader->Use();
	Select();

	glm::mat4 matrix = projViewMat * objMat;
	shader->SendUniformMatGLM("gWVP", matrix);

	std::vector<glm::mat4> transforms;

	if(animationIndex >= scene->mNumAnimations)
		animationIndex = 0;

	AnimatingFunctions::AnimationMatrix::GetBoneTransforms(transforms, animationT, scene, this, animationIndex);

	const uint size = transforms.size();

	for (uint i = 0; i < size; ++i)
	{
		std::string path = "gBones[";
		path += std::to_string(i);
		path += "]";

		shader->SendUniformMatGLM(path, transforms[i]);
	}

	int val = (int)isTextured;
	shader->SendUniformInt("displayTexture", val);

	shader->SendUniformInt("transformIndex", transformsOffset);

	shader->SendUniformFloat("timeTicks", animationT);

	const unsigned meshesSize = datas->meshes.size();
	for (unsigned i = 0; i < meshesSize; ++i)
	{
		unsigned materialIndex = datas->meshes[i].MaterialIndex;
		assert(materialIndex < datas->materials.size());

		if (datas->materials[materialIndex].pDiffuse)
		{
			datas->materials[materialIndex].pDiffuse->Bind(0);
		}

		if (datas->materials[materialIndex].pSpecular)
		{
		}

		glDrawElementsBaseVertex(GL_TRIANGLES, datas->meshes[i].NumIndices, GL_UNSIGNED_INT,
			(void*)(sizeof(unsigned int) * datas->meshes[i].BaseIndex),
			datas->meshes[i].BaseVertex);

	}

	glBindVertexArray(0);

}

aiNode* AnimationModel::GetRootNode()
{
	return rootNode;
}

const aiScene* AnimationModel::GetScene()
{
	return scene;
}

void AnimationModel::PopulateTransforms(std::vector<glm::mat4>& transforms)
{
	datas->PopulateTransforms(vao, transforms);
}


