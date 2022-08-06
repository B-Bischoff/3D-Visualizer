#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <iostream>
#include <vector>

class Camera {
public:
	glm::vec3 position;
	glm::vec3 front, up, right;
	glm::vec3 worldUp;

	glm::vec3 targetPos = {0, 0, 0};

	float translationSpeed = 2.0f;
	float rotationSpeed = 4.0f;
	float zoomSpeed = 0.1f;

	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 getViewMatrix() const;

	void processZoom(float yoffset);
	void processRotations(float xAngle, float yAngle);
	void processTranslations(float xVelocity, float yVelocity);

private:
	void updateCameraVectors();
};

