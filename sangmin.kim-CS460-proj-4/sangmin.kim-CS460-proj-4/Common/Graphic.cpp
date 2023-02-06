/*
 * Author		: Ryan Kim.
 * Date			: 2022-10-07
 * Description	: Class for openGL calls.
 */

#include "Graphic.h"
#include <chrono>
#include <assimp/scene.h>

#include "Shader.h"
#include "Camera.hpp"
#include <GLFW/glfw3.h>
#include "AnimationModel.h"
#include "Object.h"
#include <fstream>

#include "Buffer.hpp"
#include "Line.h"
#include "PhysicsSimulation.h"
#include "Pointmass.h"
#include "SimpleBox.h"
#include "Skybox.h"
#include "Texture.h"


Graphic::Graphic() : windowWidth(128 * 10), windowHeight(128 * 6), deltaTime(0.f), lastFrame(0.f)
{
	std::cout << "Graphic()" << std::endl;
	shader = new Shader("../Shaders/vert.glsl", "../Shaders/frag.glsl");
	lineShader = new Shader("../Shaders/lineVert.glsl", "../Shaders/lineFrag.glsl");
	floorShader = new Shader("../Shaders/floorVertex.glsl", "../Shaders/floorFragment.glsl");
	dotsShader = new Shader("../Shaders/SimpleVert.glsl", "../Shaders/SimpleFrag.glsl");

	boxTexture = new Texture(GL_TEXTURE_2D,	"../Models/container.jpg");
	boxTexture->Load();

	cam = new Camera(glm::vec3(8.99861f, 16.0555f, 43.1732f), 
		glm::vec3(0.0372666f, 0.906928f, -0.419634f),
		-87.1001f, -24.9f);

	line = new Line();
	InitLineBuffer();
	Populate();

	startTime = std::chrono::system_clock::now();
	transformsOffset = 0;
	animationIndex = 0;
	showOthers = false;
	skybox = new SkyBox();
	physicsSimulation = new PhysicsSimulation(dotsShader, lineShader);

	simpleBox = new SimpleBox(floorShader);
	frontRight = new SimpleBox(floorShader);
	backRight = new SimpleBox(floorShader);
	frontLeft = new SimpleBox(floorShader);
	backLeft = new SimpleBox(floorShader);
	Reset();
}

Graphic::~Graphic()
{
	delete shader;
	delete lineShader;
	delete cam;
	delete line;
	delete skybox;
	delete physicsSimulation;
	delete simpleBox;
	delete frontLeft;
	delete frontRight;
	delete backLeft;
	delete backRight;
}

void Graphic::Populate()
{

}



void Graphic::Draw(float dt)
{
	glClearColor(0.5f, .5f, .5f, 1.f);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, windowWidth, windowHeight);
	glLineWidth(10.F);
	const glm::mat4 projMat = glm::perspective(glm::radians(cam->Zoom), (float)windowWidth / (float)windowHeight,
	                                           0.1f, 1000.f);
	const glm::mat4 viewMat = cam->GetViewMatrix();
	const glm::mat4 projViewMat = projMat * viewMat;

	skybox->Draw(projMat, viewMat);

	physicsSimulation->UpdateSimulation(dt, simpleBox);
	physicsSimulation->Draw(projViewMat);
	simpleBox->Draw(projViewMat, boxTexture);

	physicsSimulation->SetAnchorPositions(frontLeft->pos, backLeft->pos, frontRight->pos, backRight->pos);

	frontRight->Draw(projViewMat, boxTexture);
	backRight->Draw(projViewMat, boxTexture);
	frontLeft->Draw(projViewMat, boxTexture);
	backLeft->Draw(projViewMat, boxTexture);
}

void Graphic::DrawLine(glm::mat4 projViewMat_)
{
	lineShader->Use();
	glBindVertexArray(lineVao);
	linePosBuffer->Bind();

	lineShader->SendUniformMatGLM("gWVP", projViewMat_);

	const size_t size = line->Coords().size();

	glDrawArrays(GL_LINES, 0, static_cast<int>(size));

	glBindVertexArray(0);

}

void Graphic::ProcessInput()
{
	if(glfwGetKey(window, GLFW_KEY_W))
		cam->ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A))
		cam->ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D))
		cam->ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S))
		cam->ProcessKeyboard(BACKWARD, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_1))
		animationIndex = 0;
	if (glfwGetKey(window, GLFW_KEY_2))
		animationIndex = 1;
	if (glfwGetKey(window, GLFW_KEY_3))
		animationIndex = 2;
	if (glfwGetKey(window, GLFW_KEY_4))
		animationIndex = 3;


	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camLock = !camLock;
}

void Graphic::InitLineBuffer()
{
	lineCoords = line->Coords();

	const size_t coordsSize = lineCoords.size();

	glGenVertexArrays(1, &lineVao);
	glBindVertexArray(lineVao);

	linePosBuffer = new Buffer(GL_ARRAY_BUFFER, coordsSize * sizeof(glm::vec3), GL_STATIC_DRAW, lineCoords.data());
	linePosBuffer->Bind();
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), static_cast<GLvoid*>(0));

	glBindVertexArray(0);

}

float Graphic::DistanceTimeFunction(float t) const
{
	float s;

	const float f = k1 * 2.f / PI + k2 - k1 + (1.f - k2) * 2.f / PI;

	if (t < k1) 
		s = k1 * (2.f / PI) * (sin(t / k1 * PI / 2.f - PI / 2.f) + 1.f);
	else if (t < k2) 
		s = 2.f * k1 / PI + t - k1;
	else 
		s = 2.f * k1 / PI + k2 - k1 + ((1.f - k2) * (2.f / PI)) * 
		sin((t - k2) / (1.f - k2) * PI / 2.f);

	return s / f;
}

void Graphic::Reset()
{
	simpleBox->pos = glm::vec3(7.f, -3.f, 7.f);
	simpleBox->scale = glm::vec3(6.f, 10.f, 6.f);
	simpleBox->rot = glm::vec3(0.f, 1.f, 0.f);

	//frontRight
	frontRight->pos = glm::vec3(15.f, 10.f, 14.f);
	frontRight->scale = glm::vec3(1.f, 1.f, 1.f);
	frontRight->rot = glm::vec3(0.f, 1.f, 0.f);

	//backRight
	backRight->pos = glm::vec3(15.f, 10.f, 0.f);
	backRight->scale = glm::vec3(1.f, 1.f, 1.f);
	backRight->rot = glm::vec3(0.f, 1.f, 0.f);

	//frontLeft
	frontLeft->pos = glm::vec3(0.f, 10.f, 14.f);
	frontLeft->scale = glm::vec3(1.f, 1.f, 1.f);
	frontLeft->rot = glm::vec3(0.f, 1.f, 0.f);

	//backLeft
	backLeft->pos = glm::vec3(0.f, 10.f, 0.f);
	backLeft->scale = glm::vec3(1.f, 1.f, 1.f);
	backLeft->rot = glm::vec3(0.f, 1.f, 0.f);

	physicsSimulation->InitializeSimulation(frontLeft->pos, backLeft->pos, frontRight->pos);

}

void Graphic::ReInitSimulation()
{
	Reset();
}
