/*
 * Author		: Ryan Kim.
 * Date			: 2022-10-07
 * Description	: Class for springsystems.
 */

#include "massspringsystem.h"

#include <iostream>

#include "Buffer.hpp"
#include "Shader.h"


MassSpringSystem::MassSpringSystem(Shader* dotShader_, Shader* lineShader_)
{
    dotShader = dotShader_;
    lineShader = lineShader_;
}

MassSpringSystem::~MassSpringSystem()
{
    for (unsigned i=0; i<masses.size(); i++) {
        delete masses[i];
    }
    for (unsigned i=0; i<springs.size(); i++) {
        delete springs[i];
    }
}

PointMass* MassSpringSystem::AddMass(float mass, float x, float y, float z)
{
    PointMass *m = new PointMass(mass, x, y, z);
    masses.push_back(m);
    return m;
}

void MassSpringSystem::AddSpring(float springConstant, float restLength,
    int mass1Index, int mass2Index, float dampingConstant)
{
    PointMass* mass1 = masses[mass1Index];
    PointMass* mass2 = masses[mass2Index];
    Spring *s = new Spring(springConstant, restLength, mass1, mass2, dampingConstant);
    mass1->springs.push_back(s);
    mass2->springs.push_back(s);
    springs.push_back(s);
}


void MassSpringSystem::update(float dt, SimpleBox* box)
{
    // update mass objects
    const unsigned massesSize = masses.size();
    for (unsigned i=0; i< massesSize; i++) 
    {
        masses[i]->update(dt, massesPositions, box);
    }

    const unsigned springSize = springs.size();
    for (unsigned i = 0; i < springSize; i++)
    {
        int index = springs[i]->index;
        int indexInVector = index * 2;
        Spring* currSpring = springs[i];
        springPositions[indexInVector] = currSpring->m1->position;
        springPositions[indexInVector + 1] = currSpring->m2->position;
    }
}

void MassSpringSystem::draw(glm::mat4 projViewMat)
{
    dotShader->Use();
    glBindVertexArray(dotShaderVao);
    dotPosBuffer->WriteData(massesPositions);
    dotPosBuffer->Bind();
    dotShader->SendUniformMatGLM("projViewModelMat", projViewMat);
    glDrawArrays(GL_POINTS, 0, massesPositions.size());
    glBindVertexArray(0);

    lineShader->Use();
    glBindVertexArray(springShaderVao);
    springPosBuffer->WriteData(springPositions);
    springPosBuffer->Bind();
    lineShader->SendUniformMatGLM("gWVP", projViewMat);
    glDrawArrays(GL_LINES, 0, springPositions.size());
    glBindVertexArray(0);
}

void MassSpringSystem::Initializing()
{
    const unsigned massesSize = masses.size();

    for (unsigned i = 0; i < massesSize; i++) {
        massesPositions.push_back(masses[i]->position);
        masses[i]->index = i;
    }

    glGenVertexArrays(1, &dotShaderVao);
    glBindVertexArray(dotShaderVao);

    dotPosBuffer = new Buffer(GL_ARRAY_BUFFER, sizeof(glm::vec3) * massesPositions.size(), GL_STATIC_DRAW,
        massesPositions.data());
    dotPosBuffer->Bind();
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, static_cast<GLvoid*>(0));

    glBindVertexArray(0);

    // draw springs
    const unsigned springsSize = springs.size();
    for (unsigned i = 0; i < springsSize; i++) 
    {
        springPositions.push_back(springs[i]->m1->position);
        springPositions.push_back(springs[i]->m2->position);
        springs[i]->index = i;
    }

    glGenVertexArrays(1, &springShaderVao);
    glBindVertexArray(springShaderVao);

    springPosBuffer = new Buffer(GL_ARRAY_BUFFER, sizeof(glm::vec3) * springPositions.size(), GL_STATIC_DRAW,
        springPositions.data());
    springPosBuffer->Bind();
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, static_cast<GLvoid*>(0));

    glBindVertexArray(0);

}