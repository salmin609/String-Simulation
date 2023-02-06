/*
 * Author		: Ryan Kim.
 * Date			: 2022-10-07
 * Description	: Class for spring
 */

#include "Spring.h"

Spring::Spring(float springConstant, float restLength,
               PointMass *mass1, PointMass *mass2, float dampingConstant)
{
    k = springConstant;
    ogLength = restLength;
    m1 = mass1;
    m2 = mass2;
    kd = dampingConstant;
}

glm::vec3 Spring::GetSpringForce(PointMass* mass)
{
    const glm::vec3 dir = normalize(m2->position - m1->position);
    const glm::vec3 springForce = HooksLaw() * dir;

    if(mass == m1)
		return springForce;
    return -springForce;
}

glm::vec3 Spring::GetDampingForce()
{
    const glm::vec3 dir = normalize(m2->position - m1->position);

    const float dT = -kd * dot(dir, (m2->velocity + m1->velocity));
    const glm::vec3 dampingForce = dT * dir;

    return dampingForce;
}

float Spring::HooksLaw()
{
    const float stretchedDistance = glm::distance(m1->position, m2->position);
    const float hooksLaw = 0.5f * k * (stretchedDistance - ogLength);

    return hooksLaw;
}

