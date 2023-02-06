/*
 * Author		: Ryan Kim.
 * Date			: 2022-10-07
 * Description	: Class for springsystems.
 */
#pragma once

#include <vector>
#include "Pointmass.h"
#include "Spring.h"

class SimpleBox;
class Shader;

class MassSpringSystem
{
public:
    MassSpringSystem(Shader* dotShader_, Shader* lineShader_);
    ~MassSpringSystem();
    PointMass* AddMass(float mass, float x, float y, float z);
    void AddSpring(float springConstant, float restLength,
                      int mass1Index, int mass2Index, float dampingConstant);


    void update(float dt, SimpleBox* box);
    void draw(glm::mat4 projViewMat);
    void Initializing();

    std::vector<PointMass*> masses;
    std::vector<glm::vec3> massesPositions;

private:
    std::vector<glm::vec3> springPositions;
    
    std::vector<Spring*> springs;
    Shader* dotShader;
	Shader* lineShader;

    unsigned dotShaderVao;
    Buffer* dotPosBuffer;

    unsigned springShaderVao;
    Buffer* springPosBuffer;
};
