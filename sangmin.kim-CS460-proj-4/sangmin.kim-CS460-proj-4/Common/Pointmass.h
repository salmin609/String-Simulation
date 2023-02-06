/*
 * Author		: Ryan Kim.
 * Date			: 2022-10-07
 * Description	: Class for mass object
 */

#pragma once

#include <vector>
#include "glm/glm.hpp"
#include "Spring.h"

class SimpleBox;
class Shader;
class Spring;
class Buffer;

class PointMass
{
public:
    PointMass(float mass, float x, float y, float z);


    glm::vec3 CalculateForces();
    void update(float dt, std::vector<glm::vec3>& massPositions,
        SimpleBox* box);
    void CalcPosition(glm::vec3 acceleration, float dt,
        std::vector<glm::vec3>& massPositions);
    bool CheckCollisionWithBox(SimpleBox* box);


    float mass; 
    float dscale;
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 gravity;
    bool isFixedPosition;
    int index;

    std::vector<Spring*> springs;
};