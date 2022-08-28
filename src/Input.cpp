#include "Input.hpp"

float g_scrollX, g_scrollY;

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

Input::Input()
	: _object(NULL), _camera(NULL), _window(NULL)
{
}

Input::Input(GLFWwindow* window, Camera* camera, Object** object)
	: _window(window), _camera(camera), _object(object)
{
}

Input::Input(Object** object)
	: _object(object), _camera(NULL), _window(NULL)
{
}

void Input::initCallbacks()
{
	glfwSetScrollCallback(_window, scroll_callback);
}

void Input::processInput()
{
	calculateDeltaTime();
	updateMousePosition();
	updateMouseVisibility();
	processCameraInput();
}

void Input::updateMousePosition()
{
	_scrollX = g_scrollX;
	_scrollY = g_scrollY;
	g_scrollX = 0;
	g_scrollY = 0;
}

void Input::calculateDeltaTime()
{
	float currentFrame = static_cast<float>(glfwGetTime());
	_deltaTime = currentFrame - _lastFrame;
	_lastFrame = currentFrame;
}

void Input::updateMouseVisibility()
{
	if (glfwGetMouseButton(_window, GLFW_MOUSE_BUTTON_LEFT) || glfwGetMouseButton(_window, GLFW_MOUSE_BUTTON_RIGHT) || glfwGetMouseButton(_window, GLFW_MOUSE_BUTTON_MIDDLE))
		glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	else
		glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Input::processCameraInput()
{
	processCameraMouse();
	processCameraScroll();
}

void Input::processCameraMouse()
{
	double cursorXPos, cursorYPos;
	glfwGetCursorPos(_window, &cursorXPos, &cursorYPos);

    float xpos = static_cast<float>(cursorXPos);
    float ypos = static_cast<float>(cursorYPos);

    if (_firstMouseUse)
    {
        _lastMouseX = xpos;
        _lastMouseY = ypos;
        _firstMouseUse = false;
    }

    float xoffset = xpos - _lastMouseX;
    float yoffset = _lastMouseY - ypos; // reversed since y-coordinates goes from bottom to top

    _lastMouseX = xpos;
    _lastMouseY = ypos;

	if (glfwGetMouseButton(_window, GLFW_MOUSE_BUTTON_MIDDLE))
		_camera->processRotations(-xoffset * _deltaTime, yoffset * _deltaTime);

	if (glfwGetMouseButton(_window, GLFW_MOUSE_BUTTON_RIGHT))
		_camera->processTranslations(xoffset * _deltaTime, -yoffset * _deltaTime);

}

void Input::processCameraScroll()
{
	_camera->processZoom(_scrollY);
}

void Input::setCamera(Camera* camera)
{
	this->_camera = camera;
}

void Input::setWindow(GLFWwindow* window)
{
	this->_window = window;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	g_scrollX = xoffset;
	g_scrollY = yoffset;
}
