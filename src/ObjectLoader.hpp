#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

class ObjectLoader {
public:
	ObjectLoader(std::string objectPathFile);

	std::vector<float> getPositions() const;
	std::vector<float> getTextureCoords() const;
	std::vector<unsigned int> getIndices() const;

private:
	std::string _objectFilecontent;
	std::vector<float> _positions; // Might add colors and textures
	std::vector<float> _textureCoords;
	std::vector<unsigned int> _indices;

	std::string ReadObjectFileContent(std::string objectPathFile);
	void ParseFileContent();
	void ParsePositions(std::stringstream& ss, std::string& temp);
	void ParseTextureCoords(std::stringstream& ss, std::string& temp);
	void ParseIndices(std::stringstream& ss, std::string& temp);
};
