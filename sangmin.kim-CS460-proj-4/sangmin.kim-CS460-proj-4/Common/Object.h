/*
 * Author		: Ryan Kim.
 * Date			: 2022-10-07
 * Description	: Class for store animating object's infos (position, rotation, scaling, animationModel)
 */

#pragma once
#include <chrono>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>


struct aiScene;
class AnimationModel;

class Object
{
public:
	Object(AnimationModel* model, glm::vec3 posVal, glm::vec3 rotVal, glm::vec3 scaleVal);
	~Object();
	glm::mat4 GetModelMatrix();
	void Draw(
		const glm::mat4& projViewMat, float animationT, int transformsOffset,
		unsigned animationIndex);
	std::chrono::system_clock::time_point GetAnimationStartTime() const;
	void ResetAnimationStartTime();
	AnimationModel* animationModel;
	glm::vec3 pos, rot, scale;
	glm::vec3 W, U, V;
private:
};