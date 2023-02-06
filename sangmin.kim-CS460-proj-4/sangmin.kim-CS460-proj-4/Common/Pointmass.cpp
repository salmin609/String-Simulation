/*
 * Author		: Ryan Kim.
 * Date			: 2022-10-07
 * Description	: Class for mass
 */

#include "Pointmass.h"
#include "Buffer.hpp"
#include "SimpleBox.h"


float dydx(float x, float y)
{
    return (x - y) / 2.f;
}


float rungeKutta(float x0, float y0, float dt)
{
    float y = y0;
    for (int i = 1; i <= 3; i++)
    {
        float k1 = dt * dydx(x0, y);
        float k2 = dt * dydx(x0 + 0.5f * dt, y + 0.5f * k1);
        float k3 = dt * dydx(x0 + 0.5f * dt, y + 0.5f * k2);
        float k4 = dt * dydx(x0 + dt, y + k3);

        y = y + (1.0f / 6.0f) * (k1 + 2.f * k2 + 2.f * k3 + k4);

        x0 = x0 + dt;
    }

    return y;
}

PointMass::PointMass(float m, float x, float y, float z)
{
    mass = m;
    dscale = 0.5f;
    position = glm::vec3(x, y, z);
    velocity = glm::vec3(0.0, 0.0, 0.0);
    gravity = glm::vec3(0.0, -9.81f, 0.0);
    isFixedPosition = false;

}

glm::vec3 PointMass::CalculateForces()
{
    glm::vec3 fg = gravity * (mass / 2.f);

	glm::vec3 fs = glm::vec3(0.f, 0.f, 0.f);
    glm::vec3 fd = glm::vec3(0.f, 0.f, 0.f);

    const unsigned springsSize = springs.size();

    for (unsigned i = 0; i < springsSize; ++i)
    {
        fs += springs[i]->GetSpringForce(this);
        fd += springs[i]->GetDampingForce();
    }

    glm::vec3 force = fg + fs + fd;
    return force;
}

void PointMass::update(float dt, std::vector<glm::vec3>& massPositions,
    SimpleBox* box)
{
    if (isFixedPosition)
	    return;

    if (CheckCollisionWithBox(box))
        return;

    glm::vec3 acc = CalculateForces() / mass;
    CalcPosition(acc, dt, massPositions);
}

void PointMass::CalcPosition(glm::vec3 acceleration, float dt,
    std::vector<glm::vec3>& massPositions)
{
    //velocity
    const glm::vec3 nextVelocity = velocity + (acceleration) * (dt);
    const float rungeKuttaVelX = rungeKutta(velocity.x, nextVelocity.x, dt);
    const float rungeKuttaVelY = rungeKutta(velocity.y, nextVelocity.y, dt);
    const float rungeKuttaVelZ = rungeKutta(velocity.z, nextVelocity.z, dt);
    const glm::vec3 rungeVel(rungeKuttaVelX, rungeKuttaVelY, rungeKuttaVelZ);

    velocity = rungeVel;
    position = position + velocity * dt;
    massPositions[index] = position;
}

bool PointMass::CheckCollisionWithBox(SimpleBox* box)
{
    //should check if this position is within x,z of box
    const glm::vec3 boxPosition = box->pos;
    const glm::vec3 boxScale = box->scale;

    const glm::vec3 offset = glm::vec3(0.2f);
    const glm::vec3 boxHalfScale = (boxScale / 2.f) + offset;

    if (position.x >= boxPosition.x - boxHalfScale.x &&
        position.x <= boxPosition.x + boxHalfScale.x &&
        position.z >= boxPosition.z - boxHalfScale.z &&
        position.z <= boxPosition.z + boxHalfScale.z)
    {
        //x,z were inside of the box.
        if (position.y <= boxPosition.y + boxHalfScale.y)
            return true;
    }
    return false;
}