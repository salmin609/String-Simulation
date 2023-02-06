/*
 * Author		: Ryan Kim.
 * Date			: 2022-10-07
 * Description	: Class for PhysicsSimulation
 */

#pragma once

class SimpleBox;
class Spring;
class PointMass;
class Shader;
class MassSpringSystem;

#include "glm/mat4x4.hpp"


class PhysicsSimulation
{
public:
	PhysicsSimulation(Shader* dotShader_, Shader* lineShader_);
	~PhysicsSimulation();
    void SetVariables();
    void InitializeSimulation(glm::vec3 leftFront, glm::vec3 leftBack, glm::vec3 rightFront);
    void UpdateSimulation(float dt, SimpleBox* box);
    void Draw(glm::mat4 projViewMat);
    void FreezeObjs(bool toggle);
    void SetAnchorPositions(glm::vec3 leftFront, glm::vec3 leftBack, glm::vec3 rightFront, glm::vec3 rightBack);
    float massValue;
    float springConstantValue;
    float dampingConstantValue;
    float restLengthValue;
private:
    MassSpringSystem* simSystem = nullptr;
    Shader* dotShader;
    Shader* lineShader;
    int leftBackIndex = 0;
    int leftFrontIndex = 0;
    int rightFrontIndex = 0;
    int rightBackIndex = 0;

    int width, height;
    int y;

};
