#include "Input.hpp"

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

void Input::processInput()
{
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
	processCameraKeyboard();
	processCameraMouse();
}

void Input::processCameraKeyboard()
{
	if (glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS)
        _camera->processKeyboard(FORWARD, _deltaTime);
    if (glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS)
        _camera->processKeyboard(BACKWARD, _deltaTime);
    if (glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS)
        _camera->processKeyboard(LEFT, _deltaTime);
    if (glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS)
        _camera->processKeyboard(RIGHT, _deltaTime);
    if (glfwGetKey(_window, GLFW_KEY_SPACE) == GLFW_PRESS)
        _camera->processKeyboard(UP, _deltaTime);
	if (glfwGetKey(_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		_camera->processKeyboard(DOWN, _deltaTime);
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
    float yoffset = _lastMouseY - ypos; // reversed since y-coordinates go from bottom to top

    _lastMouseX = xpos;
    _lastMouseY = ypos;

    _camera->processMouseMovement(xoffset, yoffset);

	//glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//processInput(_window, camera, 1.0f / ImGui::GetIO().Framerate);

}

void Input::processObjectInput()
{
	glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	
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
