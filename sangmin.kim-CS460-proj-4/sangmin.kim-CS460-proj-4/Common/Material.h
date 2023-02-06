/*
 * Author		: Ryan Kim.
 * Date			: 2022-10-07
 * Description	: class for material information.
 */

#pragma once

#include <glm/vec3.hpp>

struct Texture;

class Material
{
public:
	glm::vec3 ambientColor = glm::vec3(0.f, 0.f, 0.f);
	glm::vec3 diffuseColor = glm::vec3(0.f, 0.f, 0.f);
	glm::vec3 specularColor = glm::vec3(0.f, 0.f, 0.f);

	Texture* pDiffuse = nullptr;
	Texture* pSpecular = nullptr;

};