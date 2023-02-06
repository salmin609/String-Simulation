/*
 * Author		: Ryan Kim.
 * Date			: 2022-11-07
 * Description	: Functions for build floor object.
 */

#pragma once
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"


class Texture;
class SimpleBox;
class Shader;
class Buffer;

class Floor
{
public:
	Floor(Shader* shader_);
	~Floor();
	void Draw(const glm::mat4& projViewMat, Texture* boxTexture);
	glm::mat4 GetModelMatrix();
	

private:
	SimpleBox* box;
};
