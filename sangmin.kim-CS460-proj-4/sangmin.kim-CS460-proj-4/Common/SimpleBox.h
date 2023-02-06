/*
 * Author		: Ryan Kim.
 * Date			: 2022-10-07
 * Description	: Simple box class.
 */


#pragma once

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"

class Texture;
class Buffer;
class Shader;

class SimpleBox
{
public:
	SimpleBox(Shader* shader_);
	~SimpleBox();
	void Draw(const glm::mat4& projViewMat, Texture* texture);
	glm::mat4 GetModelMatrix();
	glm::vec3 scale, rot, pos;

private:
	unsigned vao;
	Buffer* positionBuffer;
	Buffer* texCoordBuffer;
	Shader* shader;
};
