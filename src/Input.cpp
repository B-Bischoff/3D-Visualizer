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

void Input::InitCallbacks()
{
	glfwSetScrollCallback(_window, scroll_callback);
}

void Input::processInput()
{
	_scrollX = g_scrollX;
	_scrollY = g_scrollY;
	g_scrollX = 0;
	g_scrollY = 0;

	if (glfwGetMouseButton(_window, GLFW_MOUSE_BUTTON_LEFT) || glfwGetMouseButton(_window, GLFW_MOUSE_BUTTON_RIGHT) || glfwGetMouseButton(_window, GLFW_MOUSE_BUTTON_MIDDLE))
	{
		glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	else
	{
		glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}


	//glfwSetInputMode(_window, GLFW_STICKY_KEYS, GL_TRUE);
	calculateDeltaTime();
	processCameraInput();
	processObjectInput();
}

void Input::calculateDeltaTime()
{
	float currentFrame = static_cast<float>(glfwGetTime());
	_deltaTime = currentFrame - _lastFrame;
	_lastFrame = currentFrame;
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

void Input::processObjectInput()
{
	if (glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS)
		(*_object)->rotation.x += 1;
	if (glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS)
		(*_object)->rotation.x -= 1;
	if (glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS)
		(*_object)->rotation.y -= 1;
	if (glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS)
		(*_object)->rotation.y += 1;

	if (glfwGetKey(_window, GLFW_KEY_UP) == GLFW_PRESS)
		(*_object)->translation.y += 0.05f;
	if (glfwGetKey(_window, GLFW_KEY_DOWN) == GLFW_PRESS)
		(*_object)->translation.y -= 0.05f;
	if (glfwGetKey(_window, GLFW_KEY_LEFT) == GLFW_PRESS)
		(*_object)->translation.x -= 0.05f;
	if (glfwGetKey(_window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		(*_object)->translation.x += 0.05f;
	if (glfwGetKey(_window, GLFW_KEY_Q) == GLFW_PRESS)
		(*_object)->translation.z += 0.05f;
	if (glfwGetKey(_window, GLFW_KEY_E) == GLFW_PRESS)
		(*_object)->translation.z -= 0.05f;

	if (glfwGetKey(_window, GLFW_KEY_KP_ADD) == GLFW_PRESS)
		(*_object)->scale += 0.01f;
	if (glfwGetKey(_window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS)
		(*_object)->scale -= 0.01f;
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
