#include "Grid.hpp"

Grid::Grid()
{
	std::vector<float> positions = createGridPositions();
	std::vector<unsigned int> indices = createGridIndices();
	std::vector<float> textureCoords = createGridTextureCoords(positions);

	_grid = new Object(positions, positions, textureCoords, indices);
	_grid->renderMode = Drawing_mode::WIREFRAME;
	_grid->materialType = Material_type::TEXTURE;
}

Grid::~Grid()
{
	delete _grid;
}

void Grid::render()
{
	_grid->render();
}

void Grid::setProgram(Program* program)
{
	_grid->addProgram(program);
}

std::vector<float> Grid::createGridPositions()
{
	std::vector<float> positions;

	float x = -HALF_SIDE_LENGTH;
	const float y = 0.0f;
	float z = -HALF_SIDE_LENGTH;

	const float DST = HALF_SIDE_LENGTH * 2 / (VERTEX_PER_SIDE - 1.0f);

	int side_cpt = 0;

	for (int i = 0; i < TOTAL_VERTEX_NB; i++)
	{
		if (i == VERTEX_PER_SIDE)
		{
			x = HALF_SIDE_LENGTH;
		}
		if (i == TOTAL_VERTEX_NB - VERTEX_PER_SIDE)
		{
			z = HALF_SIDE_LENGTH;
			x = -HALF_SIDE_LENGTH;
		}
		if (i < VERTEX_PER_SIDE || i > TOTAL_VERTEX_NB - VERTEX_PER_SIDE - 1)
		{
			positions.push_back(x);
			positions.push_back(y);
			positions.push_back(z);
			x += DST;
		}
		else
		{
			x = -x;
			if (side_cpt == 0)
				z += DST;
			positions.push_back(x);
			positions.push_back(y);
			positions.push_back(z);

			side_cpt = (side_cpt + 1) % 2;
		}
	}
	return positions;

}

std::vector<unsigned int> Grid::createGridIndices()
{
	std::vector<unsigned int> indices;

	createHorizontalLinesIndices(indices);
	createVerticalLinesIndices(indices);

	return indices;
}

void Grid::createHorizontalLinesIndices(std::vector<unsigned int>& indices)
{
	for (int i = 0; i < VERTEX_PER_SIDE; i++)
	{
		if (i == 0) // First line
		{
			indices.push_back(i);
			indices.push_back(VERTEX_PER_SIDE - 1);
		}
		else if (i == VERTEX_PER_SIDE - 1) // Last line
		{
			indices.push_back(VERTEX_PER_SIDE + ((i - 1) * 2));
			indices.push_back(TOTAL_VERTEX_NB - 1);
		}
		else
		{
			indices.push_back(VERTEX_PER_SIDE + ((i - 1) * 2));
			indices.push_back(VERTEX_PER_SIDE + ((i - 1) * 2) + 1);
		}
	}
}

void Grid::createVerticalLinesIndices(std::vector<unsigned int>& indices)
{
	for (int i = 0; i < VERTEX_PER_SIDE; i++)
	{
		indices.push_back(i);
		indices.push_back(i + (-4 + 3 * VERTEX_PER_SIDE));
	}
}

std::vector<float> Grid::createGridTextureCoords(std::vector<float>& p)
{
	std::vector<float> textureCoords;
	for (int i = 0; i < p.size(); i += 3)
	{
		textureCoords.push_back((p[i] + SIDE_LENGTH / 2) / SIDE_LENGTH); // X position
		textureCoords.push_back((p[i + 2] + SIDE_LENGTH / 2) / SIDE_LENGTH); // Z position
	}
	return textureCoords;
}
