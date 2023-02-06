/*
 * Author		: Ryan Kim.
 * Date			: 2022-10-07
 * Description	: Class for PhysicsSimulation
 */

#include "PhysicsSimulation.h"

#include <cmath>

#include "massspringsystem.h"

PhysicsSimulation::PhysicsSimulation(Shader* dotShader_, Shader* lineShader_)
{
    dotShader = dotShader_;
    lineShader = lineShader_;
    width = 75;
    height = 75;
    y = 10;

    SetVariables();
    //InitializeSimulation();
}

PhysicsSimulation::~PhysicsSimulation()
{
    delete simSystem;
}

void PhysicsSimulation::SetVariables()
{
    massValue = 0.05f;
    springConstantValue = 1.f;
    dampingConstantValue = 0.05f;
    restLengthValue = 0.05f;
}

void PhysicsSimulation::InitializeSimulation(glm::vec3 leftFront, glm::vec3 leftBack, glm::vec3 rightFront)
{
    delete simSystem;
    simSystem = new MassSpringSystem(dotShader, lineShader);

    
    const float xStep = (rightFront.x - leftFront.x) / static_cast<float>(width);
    const float zStep = (rightFront.z - leftBack.z) / static_cast<float>(height);

    float z = leftBack.z;
    for (int i = 0; i < height; ++i) 
    {
        float x = leftBack.x;
        for (int j = 0; j < width; ++j)
        {
            //right back
            if(i == 0 && j == width - 1)
            {
                rightBackIndex = i * width + j;
            }
            //leftFront
            else if(i == height - 1 && j == 0)
            {
                leftFrontIndex = i * width + j;
            }
            else if(i == height - 1 && j == width - 1)
            {
                rightFrontIndex = i * width + j;
            }

            simSystem->AddMass(massValue, x, (float)y, z);
            x = x + xStep;
        }
        z = z + zStep;
    }

    simSystem->masses[leftBackIndex]->isFixedPosition = true;
    simSystem->masses[leftFrontIndex]->isFixedPosition = true;
    simSystem->masses[rightFrontIndex]->isFixedPosition = true;
    simSystem->masses[rightBackIndex]->isFixedPosition = true;
    
    float k = springConstantValue;
    float kd = dampingConstantValue;
    float rl = restLengthValue;
    const float rlLong = 1.4142f * rl;

    for (int j = 0; j < height - 1; ++j) 
    {
        for (int i = 0; i < width - 1; ++i) 
        {
	        const int mIndex = j * height + i;
	        const int mRightIndex = j * height + (i + 1);
	        const int mDownIndex = (j + 1) * height + i;
	        const int mDownRightIndex = (j + 1) * height + (i + 1);

            simSystem->AddSpring(k, rl, mIndex, mRightIndex, kd);
            simSystem->AddSpring(k, rl, mIndex, mDownIndex, kd);
            simSystem->AddSpring(k, rlLong, mIndex, mDownRightIndex, kd);
            simSystem->AddSpring(k, rlLong, mRightIndex, mDownIndex, kd);
        }
    }

    //FreezeObjs(true);

    simSystem->Initializing();
}

void PhysicsSimulation::UpdateSimulation(float dt, SimpleBox* box)
{
    simSystem->update(dt, box);
}

void PhysicsSimulation::Draw(glm::mat4 projViewMat)
{
    simSystem->draw(projViewMat);
}

void PhysicsSimulation::FreezeObjs(bool toggle)
{
    for(int i = 0; i < height; ++i)
    {
	    for(int j = 0; j < width; ++j)
	    {
            simSystem->masses[i * width + j]->isFixedPosition = toggle;
	    }
    }
}

void PhysicsSimulation::SetAnchorPositions(glm::vec3 leftFront, glm::vec3 leftBack, glm::vec3 rightFront,
	glm::vec3 rightBack)
{
    simSystem->massesPositions[leftBackIndex] = leftBack;
    simSystem->masses[leftBackIndex]->position = leftBack;

    simSystem->massesPositions[leftFrontIndex] = leftFront;
    simSystem->masses[leftFrontIndex]->position = leftFront;

    simSystem->massesPositions[rightFrontIndex] = rightFront;
    simSystem->masses[rightFrontIndex]->position = rightFront;

    simSystem->massesPositions[rightBackIndex] = rightBack;
    simSystem->masses[rightBackIndex]->position = rightBack;
}
