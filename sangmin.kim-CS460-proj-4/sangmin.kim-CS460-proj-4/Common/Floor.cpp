/*
 * Author		: Ryan Kim.
 * Date			: 2022-11-07
 * Description	: Functions for build floor object.
 */

#include "Floor.hpp"
#include "Buffer.hpp"
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"
#include "SimpleBox.h"

Floor::Floor(Shader* shader_)
{
	box = new SimpleBox(shader_);
}

Floor::~Floor()
{
	delete box;
}

void Floor::Draw(const glm::mat4& projViewMat, Texture* boxTexture)
{
	box->Draw(projViewMat, boxTexture);
}