#include "Object.hpp"

Object::Object(ObjectLoader objectLoader)
	: VERTEX_NUMBER(objectLoader.getPositions().size()), NB_OF_VERTEX_TO_DRAW(objectLoader.getIndices().size())
{
	translation = glm::vec3(0.0f);
	rotation = glm::vec3(1.0f);
	scale = glm::vec3(1.0f);

	std::vector<float> positions = objectLoader.getPositions();
	std::vector<unsigned int> indices = objectLoader.getIndices();
	std::vector<float> textureCoords = objectLoader.getTextureCoords();

	InitPositions(positions);
	InitColors(positions);
	InitTextureCoords(textureCoords);
	InitIndices(indices);
	InitVao();
}

Object::Object(std::vector<float> positions, std::vector<float> colors, std::vector<float> textureCoords, std::vector<unsigned int> indices)
	: VERTEX_NUMBER(positions.size()), NB_OF_VERTEX_TO_DRAW(indices.size())
{
	translation = glm::vec3(0.0f);
	rotation = glm::vec3(1.0f);
	scale = glm::vec3(1.0f);

	InitPositions(positions);
	InitColors(colors);
	InitTextureCoords(textureCoords);
	InitIndices(indices);
	InitVao();
}

void Object::InitPositions(std::vector<float> positions)
{
	this->_positions = positions;

	glGenBuffers(1, &_positionBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, _positionBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _positions.size(), &_positions[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Object::InitColors(std::vector<float> colors)
{
	std::vector<float>colorVector;

	for (int i = 0; i < colors.size(); i += 3)
	{
		colorVector.push_back(colors[i]);
		colorVector.push_back(colors[i + 1]);
		colorVector.push_back(colors[i + 2]);
		colorVector.push_back(1.0f);
	}
	this->_colors = colorVector;
	applyColorsFromPositions();

	glGenBuffers(1, &_colorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, _colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _colors.size(), &_colors[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Object::InitIndices(std::vector<unsigned int> indices)
{
	this->_indices = indices;

	glGenBuffers(1, &_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * _indices.size(), &_indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Object::InitVao()
{
	glGenVertexArrays(1, &this->_vao);
	glBindVertexArray(this->_vao);

	glBindBuffer(GL_ARRAY_BUFFER, this->_positionBuffer);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	glBindBuffer(GL_ARRAY_BUFFER, this->_colorBuffer);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	glBindBuffer(GL_ARRAY_BUFFER, this->_textureCoordsBuffer);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_ibo);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glUseProgram(0);
}

void Object::InitTextureCoords(std::vector<float> textureCoords)
{
	if (textureCoords.size() == 0)
		return;

	this->_textureCoords = textureCoords;

	glGenBuffers(1, &_textureCoordsBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, _textureCoordsBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * textureCoords.size(), &_textureCoords[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Object::render()
{
	if (visible == false)
		return;

	if (materialType == Material_type::TEXTURE && _textureCoords.size() == 0)
		materialType = Material_type::COLOR;

	_programs[materialType]->useProgram();
	
	glm::mat4 transform = glm::mat4(1.0f);
	int transformUniform = glGetUniformLocation(_programs[materialType]->getProgramID(), "transform");


	transform = glm::translate(transform, this->translation);

	transform = glm::rotate(transform, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	transform = glm::rotate(transform, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	transform = glm::rotate(transform, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

	transform = glm::scale(transform, this->scale);
	glUniformMatrix4fv(transformUniform, 1, GL_FALSE, glm::value_ptr(transform));

	glBindVertexArray(this->_vao);

	if (materialType == Material_type::TEXTURE && _texture != NULL)
		_texture->activeTexture(*_programs[materialType], GL_TEXTURE0);

	glPointSize(10.0f);
	if (renderMode == Drawing_mode::TRIANGLES)
		glDrawElements(GL_TRIANGLES, NB_OF_VERTEX_TO_DRAW, GL_UNSIGNED_INT, 0);
	else if (renderMode == Drawing_mode::POINTS)
		glDrawElements(GL_POINTS, NB_OF_VERTEX_TO_DRAW, GL_UNSIGNED_INT, 0);
	else if (renderMode == Drawing_mode::WIREFRAME)
		glDrawElements(GL_LINES, NB_OF_VERTEX_TO_DRAW, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}

void Object::addProgram(Program* program)
{
	_programs.push_back(program);
}

void Object::applyColor()
{
	for (int i = 0; i < _positions.size(); i += 3)
	{
		_colors[i] = color.r;
		_colors[i + 1] = color.g;
		_colors[i + 2] = color.b;
		_colors[i + 3] = 1.0f;
	}

	glBindVertexArray(this->_vao);
	glBindBuffer(GL_ARRAY_BUFFER, _colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _colors.size(), &_colors[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Object::applyColorsFromPositions()
{
	for (int i = 0; i < _positions.size(); i += 3)
	{
		_colors[i] = _positions[i];
		_colors[i + 1] = _positions[i + 1];
		_colors[i + 2] = _positions[i + 2];
		_colors[i + 3] = 1.0f;
	}

	glBindVertexArray(this->_vao);
	glBindBuffer(GL_ARRAY_BUFFER, _colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _colors.size(), &_colors[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Object::applyRandomColors()
{
	srand(time(NULL));

	for (int i = 0; i < _positions.size(); i += 3)
	{
		_colors[i] = static_cast<float>(rand() % 255) / 255.0f;
		_colors[i + 1] = static_cast<float>(rand() % 255) / 255.0f;
		_colors[i + 2] = static_cast<float>(rand() % 255) / 255.0f;
		_colors[i + 3] = 1.0f;
	}

	glBindVertexArray(this->_vao);
	glBindBuffer(GL_ARRAY_BUFFER, _colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _colors.size(), &_colors[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Object::printPositions() const
{
	for (int i = 0; i < _positions.size(); i += 3)
	{
		std::cout << "Vert " << i / 3 << " |";
		std::cout << " x: " << _positions[i];
		std::cout << " y: " << _positions[i + 1];
		std::cout << " z: " << _positions[i + 2];
		std::cout << std::endl;
	}
}

void Object::printTextureCoords() const
{
	for (int i = 0; i < _textureCoords.size(); i += 2)
	{
		std::cout << "Texture coordinate: " << i / 2 << " |";
		std::cout << " u: " << _textureCoords[i];
		std::cout << " v: " << _textureCoords[i + 1];
		std::cout << std::endl;
	}
}

void Object::printIndices() const
{	
	for (int i = 0; i < _indices.size(); i += 3)
	{
		std::cout << "triangle " << i / 3 << " |";
		std::cout << " a: " << _indices[i];
		std::cout << " b: " << _indices[i + 1];
		std::cout << " c: " << _indices[i + 2];
		std::cout << std::endl;
	}
}

void Object::setTexture(TextureLoader* texture)
{
	this->_texture = texture;
}


