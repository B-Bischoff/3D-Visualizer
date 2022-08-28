#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Camera.hpp"
#include "Object.hpp"

class Input {
private:

	GLFWwindow* _window;
	Camera* _camera;
	Object** _object;

	float _deltaTime = 0.0f;
	float _lastFrame = 0.0f;

	bool _firstMouseUse = true;
	float _lastMouseX = 0, _lastMouseY = 0;
	float _scrollX = 0, _scrollY = 0;

	void updateMousePosition();
	void updateMouseVisibility();
	void calculateDeltaTime();

	void processCameraInput();
	void processCameraMouse();
	void processCameraScroll();

public:
	Input();
	Input(GLFWwindow* window, Camera* camera, Object** object);
	Input(Object** object);

	void initCallbacks();
	void processInput();

	void setCamera(Camera* camera);
	void setWindow(GLFWwindow* window);
};

