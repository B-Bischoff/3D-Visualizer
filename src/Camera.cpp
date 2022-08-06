#include "Camera.hpp"

Camera::Camera(glm::vec3 position, glm::vec3 up)
	: front(glm::vec3(0.0f, 0.0f, -1.0f))
{
	this->position = position;
	this->worldUp = up;
	updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() const
{
	return glm::lookAt(position, targetPos, up);
}

void Camera::processZoom(float yoffset)
{
	const float value = zoomSpeed * yoffset; // offset is inverted to invert the scroll

	position = position + ((targetPos - position) * value);

	updateCameraVectors();
}

void Camera::processTranslations(float xVelocity, float yVelocity)
{
	xVelocity *= translationSpeed;
	yVelocity *= translationSpeed;

	position += right * xVelocity;
	position += up * yVelocity;

	targetPos += right * xVelocity;
	targetPos += up * yVelocity;

	updateCameraVectors();
}

void Camera::processRotations(float xAngle, float yAngle) // Process camera rotation
{
	glm::vec4 pos(position.x, position.y, position.z, 1.0f);
	glm::vec4 pivot(targetPos.x, targetPos.y, targetPos.z, 1.0f);

	glm::mat4x4 rotationMatrixX(1.0f);
	rotationMatrixX = glm::rotate(rotationMatrixX, glm::radians(xAngle * rotationSpeed), up);
	pos = (rotationMatrixX * (pos - pivot)) + pivot;

	glm::mat4x4 rotationMatrixY(1.0f);
	rotationMatrixY = glm::rotate(rotationMatrixY, glm::radians(yAngle * rotationSpeed), -right);

	glm::vec3 finalPos = (rotationMatrixY * (pos - pivot)) + pivot;
	position = finalPos;

	updateCameraVectors();
}

void Camera::updateCameraVectors()
{
	const glm::mat4 inverted = glm::inverse(getViewMatrix());
	front = glm::normalize(glm::vec3(inverted[2]));
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}

