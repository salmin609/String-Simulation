/*
 * Author		: Ryan Kim.
 * Date			: 2022-10-07
 * Description	: Simple box class.
 */

#include "SimpleBox.h"

#include <vector>

#include "Buffer.hpp"
#include <GL/glew.h>
#include <glm/gtx/transform.hpp>

#include "Shader.h"
#include "Texture.h"

SimpleBox::SimpleBox(Shader* shader_)
{
	shader = shader_;
	std::vector<float>vertices = {
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
	};

	std::vector<float> texCoords;

	int verticesSize = vertices.size();
	for(int i = 0; i < verticesSize; i += 3)
	{
		float x = vertices[i];
		float y = vertices[i + 1];

		if (x < 0.f)
			texCoords.push_back(0.f);
		else
			texCoords.push_back(1.f);

		if (y < 0.f)
			texCoords.push_back(0.f);
		else
			texCoords.push_back(1.f);
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	positionBuffer = new Buffer(GL_ARRAY_BUFFER,
		vertices.size() * sizeof(float),
		GL_STATIC_DRAW,
		vertices.data());
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);

	texCoordBuffer = new Buffer(GL_ARRAY_BUFFER,
		texCoords.size() * sizeof(float),
		GL_STATIC_DRAW,
		texCoords.data());
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (GLvoid*)0);


	glBindVertexArray(0);
}

SimpleBox::~SimpleBox()
{
	delete positionBuffer;
	delete texCoordBuffer;
	glDeleteVertexArrays(1, &vao);
}

void SimpleBox::Draw(const glm::mat4& projViewMat, Texture* texture)
{
	shader->Use();
	glBindVertexArray(vao);
	texture->Bind(GL_TEXTURE0);
	glm::mat4 projViewModelMat = projViewMat * GetModelMatrix();
	shader->SendUniformMatGLM("projViewModelMat", projViewModelMat);

	glDrawArrays(GL_TRIANGLES, 0, 36);

	glBindVertexArray(0);
}

glm::mat4 SimpleBox::GetModelMatrix()
{
	glm::mat4 modelMat = glm::mat4(1.f);

	modelMat = glm::translate(modelMat, pos);
	modelMat = glm::scale(modelMat, scale);

	return modelMat;
}
