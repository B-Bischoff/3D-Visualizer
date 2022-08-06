#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include "Program.hpp"
#include "ObjectLoader.hpp"
#include "TextureLoader.hpp"

enum Drawing_mode {
	TRIANGLES,
	WIREFRAME,
	POINTS,
};

enum Material_type {
	COLOR,
	TEXTURE,
};

class Object {
public:
	glm::vec3 translation, rotation, scale;
	glm::vec4 color = {1, 1, 1, 1};
	bool visible = true;

	Object(ObjectLoader objectLoader);
	Object(std::vector<float> positions,
		std::vector<float> colors,
		std::vector<float> textureCoords,
		std::vector<unsigned int> indices);

	Drawing_mode renderMode = TRIANGLES;
	Material_type materialType = COLOR;

	void render();

	void applyColor();
	void applyRandomColors();
	void applyColorsFromPositions();

	void addProgram(Program* program);
	void setTexture(TextureLoader* texture);

	void printPositions() const;
	void printIndices() const;
	void printTextureCoords() const;

private:
	std::vector<float> _positions, _colors, _textureCoords;
	std::vector<unsigned int> _indices;
	unsigned int _positionBuffer, _colorBuffer, _textureCoordsBuffer;
	unsigned int _vao, _ibo;
	const int VERTEX_NUMBER, NB_OF_VERTEX_TO_DRAW;
	std::vector<Program*> _programs;
	TextureLoader* _texture = NULL;

	void InitPositions(std::vector<float> positions);
	void InitColors(std::vector<float> colors);
	void InitIndices(std::vector<unsigned int> indices);
	void InitTextureCoords(std::vector<float> textureCoords);
	void InitVao();
};
