/*
 * Author		: Ryan Kim.
 * Date			: 2022-10-07
 * Description	: Class for Spring.
 */
#pragma once

#include "glm/glm.hpp"
#include "Pointmass.h"

class Buffer;
class PointMass;
class Shader;

class Spring
{
public:
    Spring(float springConstant, float restLength,
           PointMass *mass1, PointMass *mass2, float dampingConstant);
    
    glm::vec3 GetSpringForce(PointMass* mass);
    glm::vec3 GetDampingForce();
    float HooksLaw();
    PointMass* m1;   
    PointMass* m2;
    int index;
    float k;
    float kd;
    float ogLength;
};