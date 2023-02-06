#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexcoords;

uniform mat4 projViewModelMat;

out vec2 texCoord;

void main()
{
	texCoord = aTexcoords;
	gl_Position = projViewModelMat * vec4(aPos, 1.0);
}