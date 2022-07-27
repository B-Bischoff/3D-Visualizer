#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>
#include "Program.hpp"
#include "ObjectLoader.hpp"

class Object {
public:
	glm::vec3 translation, rotation, scale;

	Object(ObjectLoader objectLoader);
	Object(std::vector<float> positions,
		std::vector<float> colors,
		std::vector<float> textureCoords,
		std::vector<unsigned int> indices);

	void render();

	void setProgram(Program& program);

	void printPositions() const;
	void printIndices() const;
	void printTextureCoords() const;

private:
	std::vector<float> _positions, _colors, _textureCoords;
	std::vector<unsigned int> _indices;
	unsigned int _positionBuffer, _colorBuffer, _textureCoordsBuffer;
	unsigned int _vao, _ibo;
	const int VERTEX_NUMBER, NB_OF_VERTEX_TO_DRAW;
	Program _program;

	void InitPositions(std::vector<float> positions);
	void InitColors(std::vector<float> colors);
	void InitIndices(std::vector<unsigned int> indices);
	void InitTextureCoords(std::vector<float> textureCoords);
	void InitVao();
};
