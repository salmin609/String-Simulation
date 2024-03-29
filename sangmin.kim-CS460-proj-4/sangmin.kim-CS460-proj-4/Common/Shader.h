/*
 * Author		: Ryan Kim.
 * Date			: 2022-10-07
 * Description	: Class for shader implementation.
 */


#pragma once
#include <string>
#include <GL/glew.h>
#include "glm/mat4x4.hpp"

class Shader 
{
public:
	Shader(const char* vertexPath, const char* fragPath);
	Shader(const char* computeShader);
	Shader(const char* vertex, const char* frag, const char* tessControl, const char* tessEval);
	Shader(const char* vertex, const char* frag, const char* geometry);
	unsigned Load(const char* fileName, GLenum type, bool checkError);
	unsigned GetShaderId();
	unsigned GetUniformLocation(const char* name);
	void Use();
	void SendUniformMat(std::string uniformName, void* val) const;
	void SendUniformMatGLM(std::string uniformName, const glm::mat4& val) const;
	void SendUniformInt(std::string uniformName, void* val) const;
	void SendUniformInt(std::string uniformName, int val) const;
	void SendUniformFloat(std::string uniformName, void* val) const;
	void SendUniformFloat(std::string uniformName, float val) const;
	void SendUniformVec3(std::string uniformName, void* val) const;
	void SendUniform3fv(std::string uniformName, void* val, int count) const;
	void SendUniform4fv(std::string uniformName, void* val, int count) const;
	void SendUniform1fv(std::string uniformName, void* val, int count) const;
	~Shader();
private:
	unsigned programId;
	unsigned vertexShaderId;
	unsigned fragmentShaderId;
	unsigned computeShaderId;
};
