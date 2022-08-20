#include "Camera.hpp"

Camera::Camera(glm::vec3 position, glm::vec3 up)
	: front(glm::vec3(0.0f, 0.0f, -1.0f))
{
	this->position = position;
	this->worldUp = up;
	this->up = up;

	updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() const
{
	return glm::lookAt(position, targetPos, up);
}

void Camera::processZoom(float yoffset)
{
	zoomSpeed = clamp(zoomSpeed, -0.5f, 0.5f);

	const float value = zoomSpeed * yoffset; // offset is inverted to invert the scroll
	position = position + ((targetPos - position) * value);

	updateCameraVectors();
}

void Camera::processTranslations(float xVelocity, float yVelocity)
{
	translationSpeed = clamp(translationSpeed, 0.01f, 20.0f);

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
	rotationSpeed = clamp(rotationSpeed, -00.0f, 20.0f);

	glm::vec4 pos(position.x, position.y, position.z, 1.0f);
	glm::vec4 pivot(targetPos.x, targetPos.y, targetPos.z, 1.0f);

	glm::mat4x4 rotationMatrixX(1.0f);
	rotationMatrixX = glm::rotate(rotationMatrixX, glm::radians(xAngle * rotationSpeed), up);
	pos = (rotationMatrixX * (pos - pivot)) + pivot;

	if (pos == glm::vec4(0.0f))
		pos = glm::vec4(0.001f);

	glm::mat4x4 rotationMatrixY(1.0f);
	rotationMatrixY = glm::rotate(rotationMatrixY, glm::radians(yAngle * rotationSpeed), -right);

	glm::vec3 finalPos = (rotationMatrixY * (pos - pivot)) + pivot;
	position = finalPos;

	updateCameraVectors();
}

void Camera::updateCameraVectors()
{
	clampCameraProperties();
	const glm::mat4 inverted = glm::inverse(getViewMatrix());
	front = glm::normalize(glm::vec3(inverted[2]));
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}

void Camera::clampCameraProperties()
{
	if (position == glm::vec3(0.0f))
		position = glm::vec3(0.01f);
	if (targetPos == glm::vec3(0.0f))
		targetPos == glm::vec3(0.01f);
	position = glm::clamp(position, -10000.0f, 10000.0f);
	targetPos = glm::clamp(targetPos, -10000.0f, 10000.0f);

}

float Camera::clamp(float nbr, float min, float max)
{
	if (nbr < min)
		return min;
	else if (nbr > max)
		return max;
	else return nbr;
}
