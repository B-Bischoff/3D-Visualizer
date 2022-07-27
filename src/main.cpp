/*
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <stdio.h>
#include <fstream>

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"


#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"


#ifndef STB_IMAGE_IMPLEMENTATION
 #define STB_IMAGE_IMPLEMENTATION
#endif

#include "TextureLoader.hpp"
#include "Program.hpp"
#include "Object.hpp"
#include "ObjectLoader.hpp"
#include "Camera.hpp"

const int WIN_HEIGHT = 1080;
const int WIN_WIDTH = 1920;

float lastX = WIN_WIDTH / 2.0f;
float lastY = WIN_HEIGHT / 2.0f;
bool firstMouse = true;

GLFWwindow* Init(const int WIN_WIDTH, const int WIN_HEIGHT)
{
	// Initialise GLFW
	if( !glfwInit() )
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		std::cin.get();
		exit(1);
	}

	// Window init
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	GLFWwindow* window = glfwCreateWindow( WIN_WIDTH, WIN_HEIGHT, "Tutorial 04 - Colored Cube", NULL, NULL);
	if( window == NULL ){
		std::cerr << "Failed to open GLWD window" << std::endl;
		std::cin.get();
		glfwTerminate();
		exit(1);
	}

	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		std::cerr << "Failed to initialize GLEW" << std::endl;
		std::cin.get();
		glfwTerminate();
		exit(1);
	}

	// Set input mode 

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	return window;
}

void processInput(GLFWwindow *window, Camera& camera, float deltaTime)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.processKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.processKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.processKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.processKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.processKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.processKeyboard(DOWN, deltaTime);
}

void mouse_callback(GLFWwindow* window, Camera& camera, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.processMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, Camera& camera, double xoffset, double yoffset)
{
    camera.processMouseScroll(static_cast<float>(yoffset));
}

void setViewAndProjectionMatrix(Camera& camera, Program &program)
{	
	program.useProgram();
	glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)WIN_WIDTH / (float)WIN_HEIGHT, 0.1f, 100.0f);
	program.useProgram();
	program.setMat4("projection", projection);
	glm::mat4 view = camera.getViewMatrix();
	program.setMat4("view", view);
}

int main(void)
{
#if defined(IMGUI_IMPL_OPENGL_ES2)
	const char* glsl_version = "#version 100";
#elif defined(__APPLE__)
	const char* glsl_version = "#version 150";
#else
	const char* glsl_version = "#version 130";
#endif

	bool cameraMode = false;

	GLFWwindow* window = Init(WIN_WIDTH, WIN_HEIGHT);

	//glfwSetCursorPosCallback(window, mouse_callback);
	//glfwSetScrollCallback(window, scroll_callback);



	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);


	Program program("src/shaders/shader.vert", "src/shaders/shader.frag");
	Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

	ObjectLoader cube("objs/Fox.obj");
	Object obj(cube.getPositions(), cube.getPositions(), cube.getTextureCoords(), cube.getIndices());
	obj.setProgram(program);
	

	program.useProgram();
	glEnable(GL_DEPTH_TEST);


	float lastX = WIN_WIDTH / 2.0f;
	float lastY = WIN_HEIGHT / 2.0f;
	bool firstMouse = true;

	TextureLoader texture("textures/154.jpg");
	TextureLoader texture1("textures/154.jpg");

	
	program.useProgram();

	   bool show_demo_window = true;
    bool autoRotate = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	Object* selectedObject = &obj;

	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
	{
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();


		double cursorXPos, cursorYPos;
		glfwGetCursorPos(window, &cursorXPos, &cursorYPos);



		{
			//colors[0] -= 0.005f;
			//colors[1] -= 0.005f;
			//_positions[0] -= 0.0005f;

			//glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
			//glBufferData(GL_ARRAY_BUFFER, _positions.size() * sizeof(float), &_positions, GL_STATIC_DRAW);
			//glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
			//glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
		}

		if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
			cameraMode = !cameraMode;

		if (cameraMode == false)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
				selectedObject->rotation.x += 1;
			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
				selectedObject->rotation.x -= 1;
			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
				selectedObject->rotation.y -= 1;
			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
				selectedObject->rotation.y += 1;

			if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
				selectedObject->translation.y += 0.05f;
			if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
				selectedObject->translation.y -= 0.05f;
			if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
				selectedObject->translation.x -= 0.05f;
			if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
				selectedObject->translation.x += 0.05f;
			if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
				selectedObject->translation.z += 0.05f;
			if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
				selectedObject->translation.z -= 0.05f;

			if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS)
				selectedObject->scale += 0.01f;
			if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS)
				selectedObject->scale -= 0.01f;
		}
		else
		{
			mouse_callback(window, camera, cursorXPos, cursorYPos);
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			processInput(window, camera, 1.0f / ImGui::GetIO().Framerate);
			
		}

		setViewAndProjectionMatrix(camera, program);
		

		texture.activeTexture(program, GL_TEXTURE0);

		// UI
		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("Model properties:");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
			ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
			ImGui::Checkbox("Auto rotate", &autoRotate);

			ImGui::Text("Rotation");               // Display some text (you can use a format strings too)
			ImGui::SliderFloat("X rotation", &selectedObject->rotation.x, 0.0f, 360.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::SliderFloat("Y rotation", &selectedObject->rotation.y, 0.0f, 360.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::SliderFloat("Z rotation", &selectedObject->rotation.z, 0.0f, 360.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

			ImGui::Text("Scale");               // Display some text (you can use a format strings too)
			ImGui::SliderFloat("X scale", &selectedObject->scale.x, 0.0f, 2.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::SliderFloat("Y scale", &selectedObject->scale.y, 0.0f, 2.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::SliderFloat("Z scale", &selectedObject->scale.z, 0.0f, 2.0f);            // Edit 1 float using a slider from 0.0f to 1.0f




			ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

			if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}

		obj.render();


		glBindVertexArray(0);
		glUseProgram(0);


		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


		glfwSwapBuffers(window);
		glfwPollEvents();
	}



	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}
*/

#ifndef STB_IMAGE_IMPLEMENTATION
 #define STB_IMAGE_IMPLEMENTATION
#endif
#include "Application.hpp"

int main(void)
{
	Application application(1920, 1080);
	application.Loop();
}

