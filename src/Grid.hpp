#pragma once

#include <stdio.h>
#include <vector>

#include "Object.hpp"
#include "TextureLoader.hpp"

class Grid {
private:
	const float SIDE_LENGTH = 200.0f;
	const float HALF_SIDE_LENGTH = SIDE_LENGTH / 2.0f;
	const int VERTEX_PER_SIDE = 201;
	const int TOTAL_VERTEX_NB = (VERTEX_PER_SIDE - 1) * 4;

	Object* _grid = NULL;

	std::vector<float> createGridPositions();
	std::vector<unsigned int> createGridIndices();
	void createHorizontalLinesIndices(std::vector<unsigned int>& indices);
	void createVerticalLinesIndices(std::vector<unsigned int>& indices);
	std::vector<float> createGridTextureCoords(std::vector<float>& p);

public:

	Grid();
	~Grid();
	void render();

	void setProgram(Program* program);
	void setTexture(TextureLoader* texture);
};

