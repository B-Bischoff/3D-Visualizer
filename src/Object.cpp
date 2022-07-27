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

	std::cout << "Started object init" << std::endl;
	InitPositions(positions);
	std::cout << "Positions OK" << std::endl;
	InitColors(positions);
	std::cout << "Colors OK" << std::endl;
	InitTextureCoords(textureCoords);
	std::cout << "Texture coords OK" << std::endl;
	InitIndices(indices);
	std::cout << "Indices OK" << std::endl;
	InitVao();
	std::cout << "VAO OK" << std::endl;
}

Object::Object(std::vector<float> positions, std::vector<float> colors, std::vector<float> textureCoords, std::vector<unsigned int> indices)
	: VERTEX_NUMBER(positions.size()), NB_OF_VERTEX_TO_DRAW(indices.size())
{
	translation = glm::vec3(0.0f);
	rotation = glm::vec3(1.0f);
	scale = glm::vec3(1.0f);

	std::cout << "Started object init" << std::endl;
	InitPositions(positions);
	std::cout << "Positions OK" << std::endl;
	InitColors(colors);
	std::cout << "Colors OK" << std::endl;
	InitTextureCoords(textureCoords);
	std::cout << "Texture coords OK" << std::endl;
	InitIndices(indices);
	std::cout << "Indices OK" << std::endl;
	InitVao();
	std::cout << "VAO OK" << std::endl;
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
	this->_colors = colors;

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
	_program.useProgram();
	
	glm::mat4 transform = glm::mat4(1.0f);
	int transformUniform = glGetUniformLocation(_program.getProgramID(), "transform");


	transform = glm::translate(transform, this->translation);
	transform = glm::scale(transform, this->scale);

	transform = glm::rotate(transform, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	transform = glm::rotate(transform, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	transform = glm::rotate(transform, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

	glUniformMatrix4fv(transformUniform, 1, GL_FALSE, glm::value_ptr(transform));

	glBindVertexArray(this->_vao);

	glPointSize(10.0f);
	//glDrawElements(GL_POINTS, NB_OF_VERTEX_TO_DRAW, GL_UNSIGNED_INT, 0);
	//glDrawElements(GL_LINES, NB_OF_VERTEX_TO_DRAW, GL_UNSIGNED_INT, 0);
	glDrawElements(GL_TRIANGLES, NB_OF_VERTEX_TO_DRAW, GL_UNSIGNED_INT, 0);
	
}

void Object::setProgram(Program& program)
{
	this->_program = program;
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

