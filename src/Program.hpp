#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <exception>

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Program {
public:
	Program();
	Program(std::string vertexPathFile, std::string fragmentPathFile);
    ~Program();

	GLuint getProgramID() const;
	void useProgram();

    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setVec2(const std::string& name, const glm::vec2& value) const;
    void setVec2(const std::string& name, float x, float y) const;
    void setVec3(const std::string& name, const glm::vec3& value) const;
    void setVec3(const std::string& name, float x, float y, float z) const;
    void setVec4(const std::string& name, const glm::vec4& value) const;
    void setVec4(const std::string& name, float x, float y, float z, float w) const;
    void setMat2(const std::string& name, const glm::mat2& mat) const;
    void setMat3(const std::string& name, const glm::mat3& mat) const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;

private:
	GLuint _programID;
	GLuint _vertexShaderID, _fragmentShaderID;

	std::string readShaderContent(std::string file);
	void CompileShader(std::string shaderContent, GLuint& shaderID);
	void LinkProgram();
};

