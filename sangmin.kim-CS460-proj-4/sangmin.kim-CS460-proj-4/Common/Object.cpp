/*
 * Author		: Ryan Kim.
 * Date			: 2022-10-07
 * Description	: Class for store animating object's infos (position, rotation, scaling, animationModel)
 */

#include "Object.h"

#include <glm/gtc/matrix_transform.hpp>
#include "AnimationModel.h"

Object::Object(AnimationModel* model, glm::vec3 posVal, glm::vec3 rotVal, glm::vec3 scaleVal)
{
	animationModel = model;
	pos = posVal;
	rot = rotVal;
	scale = scaleVal;
}

Object::~Object()
{
}

glm::mat4 Object::GetModelMatrix()
{
	glm::mat4 modelMat = glm::mat4(1.f);

	modelMat = glm::translate(modelMat, pos);

	glm::mat4 rotMat = glm::mat4(1.f);
	rotMat[0][0] = U.x;
	rotMat[0][1] = U.y;
	rotMat[0][2] = U.z;

	rotMat[1][0] = V.x;
	rotMat[1][1] = V.y;
	rotMat[1][2] = V.z;

	rotMat[2][0] = W.x;
	rotMat[2][1] = W.y;
	rotMat[2][2] = W.z;

	modelMat = modelMat * rotMat;

	glm::mat4 scaleMat = glm::mat4(1.f);

	scaleMat[0][0] = scale.x;
	scaleMat[1][1] = scale.y;
	scaleMat[2][2] = scale.z;

	modelMat = modelMat * scaleMat;
	return modelMat;
}


void Object::Draw(
	const glm::mat4& projViewMat, float animationT, int transformsOffset,
	unsigned animationIndex)
{
	animationModel->Draw(GetModelMatrix(), projViewMat, animationT, transformsOffset,
		animationIndex);
}

std::chrono::system_clock::time_point Object::GetAnimationStartTime() const
{
	return animationModel->startTime;
}

void Object::ResetAnimationStartTime()
{
	animationModel->startTime = std::chrono::system_clock::now();
}


