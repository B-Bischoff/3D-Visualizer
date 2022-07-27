#include "Application.hpp"

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

void setViewAndProjectionMatrix(Camera& camera, Program& program)
{	
	program.useProgram();
	glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)WIN_WIDTH / (float)WIN_HEIGHT, 0.1f, 100.0f);
	program.useProgram();
	program.setMat4("projection", projection);
	glm::mat4 view = camera.getViewMatrix();
	program.setMat4("view", view);
}

Application::Application(const int winWidth, const int winHeight)
	: WIN_WIDTH(winWidth), WIN_HEIGHT(winHeight)
{
	GlfwInit();
	WindowInit();
	GlewInit();
	ImGuiInit();

	SetInputMode();


	//glfwSetCursorPosCallback(window, mouse_callback);
	//glfwSetScrollCallback(window, scroll_callback);


	_camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));
	_program.push_back(new Program("src/shaders/shader.vert", "src/shaders/shader.frag"));

	ObjectLoader ol("objs/cube.obj");

	//Object* obj = new Object(object.getPositions(), object.getPositions(), object.getTextureCoords(), object.getIndices());
	Object* obj = new Object(ObjectLoader("objs/Bunny.obj"));
	obj->setProgram(*_program[TEXTURE_SHADER]);
	_objects.push_back(obj);

	//Object* obj1 = new Object(object.getPositions(), object.getPositions(), object.getTextureCoords(), object.getIndices());
	Object* obj1 = new Object(ol);
	obj1->setProgram(*_program[TEXTURE_SHADER]);
	_objects.push_back(obj1);

	_input = Input(&_selectedObject);
	_input.setCamera(_camera);
	_input.setWindow(_window);
	
}

Application::~Application()
{
	delete _camera;
	for (int i = 0; i < _program.size(); i++)
		delete _program[i];
	for (int i = 0; i < _objects.size(); i++)
		delete _objects[i];
}

void Application::GlfwInit()
{
	if(!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		std::cin.get();
		exit(1);
	}
}

void Application::WindowInit()
{
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	this->_window = glfwCreateWindow( WIN_WIDTH, WIN_HEIGHT, "3D-Visualizer", NULL, NULL);
	if (_window == NULL)
	{
		std::cerr << "Failed to open GLFW window" << std::endl;
		std::cin.get();
		glfwTerminate();
		exit(1);
	}

	glfwMakeContextCurrent(_window);
}

void Application::GlewInit()
{
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		std::cin.get();
		glfwTerminate();
		exit(1);
	}
}

void Application::SetInputMode()
{
	glfwSetInputMode(_window, GLFW_STICKY_KEYS, GL_TRUE);
}

void Application::ImGuiInit()
{
#if defined(IMGUI_IMPL_OPENGL_ES2)
	const char* glsl_version = "#version 100";
#elif defined(__APPLE__)
	const char* glsl_version = "#version 150";
#else
	const char* glsl_version = "#version 130";
#endif

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(_window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
}

void Application::Loop()
{
	_program[TEXTURE_SHADER]->useProgram();


	_program[TEXTURE_SHADER]->useProgram();

	_selectedObject = _objects[0];

	int Selected = 0;

	bool cameraMode = false;
	TextureLoader texture("textures/156.jpg"); // Attach it to object ?

	glEnable(GL_DEPTH_TEST);
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	while (glfwGetKey(_window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
	{
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		_input.processInput();

		_program[TEXTURE_SHADER]->useProgram();

		{
			//colors[0] -= 0.005f;
			//colors[1] -= 0.005f;
			//_positions[0] -= 0.0005f;

			//glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
			//glBufferData(GL_ARRAY_BUFFER, _positions.size() * sizeof(float), &_positions, GL_STATIC_DRAW);
			//glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
			//glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
		}

		if (glfwGetKey(_window, GLFW_KEY_C) == GLFW_PRESS)
			cameraMode = !cameraMode;

		if (cameraMode == false)
		{
			glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

			if (glfwGetKey(_window, GLFW_KEY_SPACE) == GLFW_PRESS)
			{
				if (Selected == 0)
					Selected = 1;
				else
					Selected = 0;
				_selectedObject = _objects[Selected];

			}
		}

		setViewAndProjectionMatrix(*_camera, *_program[TEXTURE_SHADER]);

		texture.activeTexture(*_program[TEXTURE_SHADER], GL_TEXTURE0);

		// UI
		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("Model properties:");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

			ImGui::Text("Rotation");               // Display some text (you can use a format strings too)
			ImGui::SliderFloat("X rotation", &_selectedObject->rotation.x, 0.0f, 360.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::SliderFloat("Y rotation", &_selectedObject->rotation.y, 0.0f, 360.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::SliderFloat("Z rotation", &_selectedObject->rotation.z, 0.0f, 360.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

			ImGui::Text("Scale");               // Display some text (you can use a format strings too)
			ImGui::SliderFloat("X scale", &_selectedObject->scale.x, 0.0f, 2.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::SliderFloat("Y scale", &_selectedObject->scale.y, 0.0f, 2.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::SliderFloat("Z scale", &_selectedObject->scale.z, 0.0f, 2.0f);            // Edit 1 float using a slider from 0.0f to 1.0f




			ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

			if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}

		for (int i = 0; i < _objects.size(); i++)
			_objects[i]->render();

		glBindVertexArray(0);
		glUseProgram(0);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(_window);
		glfwPollEvents();
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
}
