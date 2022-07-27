#include "ObjectLoader.hpp"

ObjectLoader::ObjectLoader(std::string objectPathFile)
{
	_positions = std::vector<float>();
	_textureCoords = std::vector<float>();
	_indices = std::vector<unsigned int>();

	this->_objectFilecontent = ReadObjectFileContent(objectPathFile);
	ParseFileContent();
}

std::string ObjectLoader::ReadObjectFileContent(std::string objectPathFile)
{
	std::string fileContent;
	std::ifstream fileStream(objectPathFile, std::ios::in);
	
	if (fileStream.is_open())
	{
		std::stringstream sstr;
		sstr << fileStream.rdbuf();
		fileContent = sstr.str();
		fileStream.close();
	}
	else
	{
		std::cerr << "[ObjectLoader] Can't open file: " << objectPathFile << std::endl;
		std::cin.get();
		exit(1);
	}
	return fileContent;
}

void ObjectLoader::ParseFileContent()
{
	const std::string POSITION_ID = "v";
	const std::string FACE_ID = "f";
	const std::string TEXTURE_COORD_ID = "vt";

	std::stringstream ss;
	std::string temp;

	ss << this->_objectFilecontent;
	ss >> temp;


	while (!ss.eof())
	{
		if (temp == POSITION_ID)
			ParsePositions(ss, temp);
		else if (temp == TEXTURE_COORD_ID)
			ParseTextureCoords(ss, temp);
		else if (temp == FACE_ID)
			ParseIndices(ss, temp);
		else
			ss >> temp;
	}
}

void ObjectLoader::ParsePositions(std::stringstream& ss, std::string& temp)
{
	temp = "";
	for (int i = 0; i < 3; i++)
	{
		float pos;
		ss >> pos;
		this->_positions.push_back(pos);
	}
}

void ObjectLoader::ParseTextureCoords(std::stringstream& ss, std::string& temp)
{
	temp = "";
	for (int i = 0; i < 2; i++)
	{
		float coord;
		ss >> coord;
		this->_textureCoords.push_back(coord);
	}
}

void ObjectLoader::ParseIndices(std::stringstream& ss, std::string& temp)
{
	temp = "";
	std::vector<int> indicesList;
	int currentIndice = 0;
	while (temp != "f" && !ss.eof())
	{
		ss >> temp;
		if (temp == "f")
			break;
			
		indicesList.push_back(atoi(temp.c_str()) - 1);
		if (currentIndice > 1)
		{
			this->_indices.push_back(indicesList[0]);
			this->_indices.push_back(indicesList[currentIndice - 1]);
			this->_indices.push_back(indicesList[currentIndice]);
		}
		currentIndice++;
	}
}

std::vector<float> ObjectLoader::getPositions() const
{
	return _positions;
}

std::vector<float> ObjectLoader::getTextureCoords() const
{
	return _textureCoords;
}

std::vector<unsigned int> ObjectLoader::getIndices() const
{
	return _indices;
}
