#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <stdio.h>
#include <fstream>
#include <ctime>
#include <chrono>
#include <cmath>

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Input.hpp"
#include "TextureLoader.hpp"
#include "Program.hpp"
#include "Object.hpp"
#include "ObjectLoader.hpp"
#include "Camera.hpp"
#include "UserInterface.hpp"
#include "Grid.hpp"

enum ApplicationShader {
	TEXTURE_SHADER,
	COLOR_SHADER,
};

class Application {
private:
	const int WIN_WIDTH, WIN_HEIGHT;
	GLFWwindow* _window;
	const char* _glsl_version;

	Input _input;
	Camera* _camera;
	UserInterface _ui;

	std::vector<Program*> _program;
	std::vector<Object*> _objects;
	std::vector<TextureLoader*> _textures;
	Object* _selectedObject;
	Grid* _backgroundGrid;

	std::string _objectToInstantiate;

	std::time_t endTime;

	// Init
	void GlfwInit();
	void WindowInit();
	void GlewInit();
	void ImGuiInit();
	void SetInputMode();

	void instantiateObject();


public:
	Application(const int winWidth, const int winHeight);
	~Application();

	void Loop();

};

