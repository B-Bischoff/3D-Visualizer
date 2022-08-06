#include "Application.hpp"

const int WIN_HEIGHT = 1080;
const int WIN_WIDTH = 1920;


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
		std::cerr << "Failed to open GLWF window" << std::endl;
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
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WIN_WIDTH / (float)WIN_HEIGHT, 0.1f, 500.0f);
	program.useProgram();
	program.setMat4("projection", projection);
	glm::mat4 view = camera.getViewMatrix();
	program.setMat4("view", view);
}

Application::Application(const int winWidth, const int winHeight)
	: WIN_WIDTH(winWidth), WIN_HEIGHT(winHeight), _ui(winWidth, winHeight)
{
	GlfwInit();
	WindowInit();
	GlewInit();
	ImGuiInit();

	SetInputMode();

	glEnable(GL_DEPTH_TEST);
	_camera = new Camera(glm::vec3(0.0f, 0.0f, 20.0f));
	_program.push_back(new Program("src/shaders/shader.vert", "src/shaders/shaderTexture.frag"));
	_program.push_back(new Program("src/shaders/shader.vert", "src/shaders/shaderColor.frag"));

	Object* obj = new Object(ObjectLoader("objs/Bunny.obj"));
	obj->addProgram(_program[COLOR_SHADER]);
	obj->addProgram(_program[TEXTURE_SHADER]);
	_objects.push_back(obj);

	/*
	Object* obj2 = new Object(ObjectLoader("E:/Prog/Projets/OpenGL/Discover/Discover/Discover/objs/42.obj"));
	obj2->addProgram(*_program[COLOR_SHADER]);
	obj2->addProgram(*_program[TEXTURE_SHADER]);
	_objects.push_back(obj2);
	*/
	Object* obj1 = new Object(ObjectLoader("objs/Fox.obj"));
	obj1->addProgram(_program[COLOR_SHADER]);
	obj1->addProgram(_program[TEXTURE_SHADER]);
	obj1->renderMode = Drawing_mode::WIREFRAME;
	obj1->translation = glm::vec3(3, 0, 0);
	_objects.push_back(obj1);

	// Input manager
	_input = Input(&_selectedObject);
	_input.setCamera(_camera);
	_input.setWindow(_window);
	_input.InitCallbacks();

	// User interface
	_ui.setCamera(_camera);
	_ui.setObject(&_selectedObject);
	_ui.setObjectList(&_objects);
	_ui.setTexturesList(&_textures);
	_ui.setObjectToInstantiate(&_objectToInstantiate);


	// Background grid
	_backgroundGrid = new Grid();
	_backgroundGrid->setProgram(_program[COLOR_SHADER]);
	_backgroundGrid->setProgram(_program[TEXTURE_SHADER]);

}

Application::~Application()
{
	delete _camera;
	delete _backgroundGrid;
	for (int i = 0; i < _program.size(); i++)
		delete _program[i];
	for (int i = 0; i < _objects.size(); i++)
		delete _objects[i];
	for (int i = 0; i < _textures.size(); i++)
		delete _textures[i];
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
	_selectedObject = _objects[0];

	int Selected = 0;

	bool cameraMode = false;
	//TextureLoader texture("textures/154.jpg"); // Attach it to object ?
	TextureLoader textureGrid("textures/grid.png"); // Attach it to object ?

	while (glfwGetKey(_window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
	{
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (_objectToInstantiate.empty() == false)
			instantiateObject();

		_ui.createNewFrame();

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
	glfwSetInputMode(_window, GLFW_STICKY_KEYS, GL_FALSE);

			if (glfwGetKey(_window, GLFW_KEY_SPACE) == GLFW_PRESS)
			{
				if (Selected == 0)
					Selected = 1;
				else
					Selected = 0;
	//			_selectedObject = _objects[Selected];

			}
		}

		setViewAndProjectionMatrix(*_camera, *_program[TEXTURE_SHADER]);
		setViewAndProjectionMatrix(*_camera, *_program[COLOR_SHADER]);


		_ui.update();

		//texture.activeTexture(*_program[COLOR_SHADER], GL_TEXTURE0);
		for (int i = 0; i < _objects.size(); i++)
			_objects[i]->render();

		textureGrid.activeTexture(*_program[TEXTURE_SHADER], GL_TEXTURE0);
		_backgroundGrid->render();

		glBindVertexArray(0);
		glUseProgram(0);

		_ui.renderUI();

		glfwSwapBuffers(_window);
		glfwPollEvents();
	}

	_ui.shutdown();

	glfwTerminate();
}

void Application::instantiateObject()
{
	std::string logMessage;

	/*
	char buf[26];
	time_t ltime;
	time(&ltime);
	ctime_r(buf, 26, &ltime);
	buf[20] = '\0'; // Erase year

	logMessage += buf + 11; // Skip date
	*/

	try
	{
		if (_objectToInstantiate.find(".obj", _objectToInstantiate.length() - 4) != std::string::npos)
		{
			ObjectLoader objectLoader(_objectToInstantiate);
			if (objectLoader.getIncoherentFaceIndex())
				logMessage += "Warning, object has incoherent face index\n";
			Object* obj = new Object(objectLoader);
			obj->addProgram(_program[COLOR_SHADER]);
			obj->addProgram(_program[TEXTURE_SHADER]);
			_objects.push_back(obj);
			logMessage += "Successfuly loaded object: " + _objectToInstantiate + "\n";
			logMessage += "Vertex number: " + std::to_string((int)objectLoader.getVertexNumber()) + "\n";
			logMessage += "Face number: " + std::to_string((int)objectLoader.getFaceNumber()) + "\n";
			logMessage += "Texture coordinate: " + std::to_string((int)objectLoader.getTextureCoordinateNumber()) + "\n";
		}
		else
		{
			TextureLoader* texture = new TextureLoader(_objectToInstantiate);
			_textures.push_back(texture);
			logMessage += "Successfuly loaded texture: " + _objectToInstantiate;
		}
	}
	catch (std::invalid_argument& e)
	{
		logMessage += e.what();
		logMessage += " ";
		logMessage += _objectToInstantiate;
	}
	std::cout << _textures.size() << std::endl;

	_ui.addToLogs(logMessage);
	_objectToInstantiate.clear();
}


