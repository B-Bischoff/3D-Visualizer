#include "Program.hpp"

Program::Program()
{
	return;
}

Program::~Program()
{
	glDeleteProgram(this->_programID);
}

Program::Program(std::string vertexPathFile, std::string fragmentPathFile)
{
	this->_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	this->_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	std::string vertexShaderContent = readShaderContent(vertexPathFile);
	std::string fragmentShaderContent = readShaderContent(fragmentPathFile);

	CompileShader(vertexShaderContent, this->_vertexShaderID);
	CompileShader(fragmentShaderContent, this->_fragmentShaderID);

	LinkProgram();
}

std::string Program::readShaderContent(std::string file)
{
	std::string fileContent;
	std::ifstream fileStream(file, std::ios::in);
	
	if (fileStream.is_open())
	{
		std::stringstream sstr;
		sstr << fileStream.rdbuf();
		fileContent = sstr.str();
		fileStream.close();
	}
	else
	{
		throw std::invalid_argument("[Program] Can't open shader " + file);
	}
	return fileContent;
}

void Program::CompileShader(std::string shaderContent, GLuint& shaderID)
{
	char const* shaderSourcePointer = shaderContent.c_str();
	glShaderSource(shaderID, 1, &shaderSourcePointer, NULL);
	glCompileShader(shaderID);

	GLint result = GL_FALSE;
	int infoLogLength;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0)
	{
		std::vector<char> VertexShaderErrorMessage(infoLogLength + 1);
		glGetShaderInfoLog(shaderID, infoLogLength, NULL, &VertexShaderErrorMessage[0]);
		std::cerr << &VertexShaderErrorMessage[0] << std::endl;
		exit (1);
	}
}

void Program::LinkProgram()
{
	this->_programID = glCreateProgram();
	glAttachShader(_programID, _vertexShaderID);
	glAttachShader(_programID, _fragmentShaderID);
	glLinkProgram(_programID);

	GLint result = GL_FALSE;
	GLint infoLogLength;
	glGetProgramiv(_programID, GL_LINK_STATUS, &result);
	glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0)
	{
		std::vector<char> ProgramErrorMessage(infoLogLength + 1);
		glGetProgramInfoLog(_programID, infoLogLength, NULL, &ProgramErrorMessage[0]);
		std::cerr << &ProgramErrorMessage[0] << std::endl;
		exit(1);
	}

	glDetachShader(_programID, _vertexShaderID);
	glDetachShader(_programID, _fragmentShaderID);

	glDeleteShader(_vertexShaderID);
	glDeleteShader(_fragmentShaderID);
}

GLuint Program::getProgramID() const
{
	return this->_programID;
}

void Program::useProgram()
{
	glUseProgram(this->_programID);
}

void Program::setBool(const std::string &name, bool value) const
{         
	glUniform1i(glGetUniformLocation(_programID, name.c_str()), (int)value); 
}
void Program::setInt(const std::string &name, int value) const
{ 
	glUniform1i(glGetUniformLocation(_programID, name.c_str()), value); 
}
void Program::setFloat(const std::string &name, float value) const
{ 
	glUniform1f(glGetUniformLocation(_programID, name.c_str()), value); 
}
void Program::setVec2(const std::string &name, const glm::vec2 &value) const
{ 
	glUniform2fv(glGetUniformLocation(_programID, name.c_str()), 1, &value[0]); 
}
void Program::setVec2(const std::string &name, float x, float y) const
{ 
	glUniform2f(glGetUniformLocation(_programID, name.c_str()), x, y); 
}
void Program::setVec3(const std::string &name, const glm::vec3 &value) const
{ 
	glUniform3fv(glGetUniformLocation(_programID, name.c_str()), 1, &value[0]); 
}
void Program::setVec3(const std::string &name, float x, float y, float z) const
{ 
	glUniform3f(glGetUniformLocation(_programID, name.c_str()), x, y, z); 
}
void Program::setVec4(const std::string &name, const glm::vec4 &value) const
{ 
	glUniform4fv(glGetUniformLocation(_programID, name.c_str()), 1, &value[0]); 
}
void Program::setVec4(const std::string &name, float x, float y, float z, float w) const
{ 
	glUniform4f(glGetUniformLocation(_programID, name.c_str()), x, y, z, w); 
}
void Program::setMat2(const std::string &name, const glm::mat2 &mat) const
{
	glUniformMatrix2fv(glGetUniformLocation(_programID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
void Program::setMat3(const std::string &name, const glm::mat3 &mat) const
{
	glUniformMatrix3fv(glGetUniformLocation(_programID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
void Program::setMat4(const std::string &name, const glm::mat4 &mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(_programID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

