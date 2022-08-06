#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <iostream>
#include <vector>

#include "Object.hpp"
#include "Camera.hpp"

class UserInterface {
private:
	const int WIN_WIDTH, WIN_HEIGHT;

	Object** _object = NULL; // Points on Application selected object
	Camera* _camera = NULL;
	std::string* _objectToInstantiate = NULL;
	std::string _logs;

	std::vector<Object*>* _objectList = NULL;
	std::vector<TextureLoader*>* _textureList = NULL;

	void updateObjetMenu();
	void updateHierarchy();
	void updateCamera();
	void updateInstantiate();

public:
	UserInterface(const int winWidth, const int winHeight);

	void update();
	void createNewFrame() const;
	void renderUI() const;
	void shutdown() const;

	void addToLogs(std::string& string);

	void setObject(Object** _object);
	void setCamera(Camera* camera);
	void setObjectList(std::vector<Object*>* objectList);
	void setTexturesList(std::vector<TextureLoader*>* textureList);
	void setObjectToInstantiate(std::string* string);
};

