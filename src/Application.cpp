#include "Application.hpp"

const int WIN_HEIGHT = 1080;
const int WIN_WIDTH = 1920;

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

	glEnable(GL_DEPTH_TEST);
	_camera = new Camera(glm::vec3(0.0f, 0.0f, 20.0f));

	shaderInit();
	inputManagerInit();
	uiInit();
	backgroundGridInit();
	
	loadDefaultObject();
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
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

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

void Application::shaderInit()
{
	try {
		_program.push_back(new Program(ROOT_PATH + "src/shaders/shader.vert", ROOT_PATH + "src/shaders/shaderTexture.frag"));
		_program.push_back(new Program(ROOT_PATH + "src/shaders/shader.vert", ROOT_PATH + "src/shaders/shaderColor.frag"));
	}
	catch (std::exception& e) {}
	try {
		_program.push_back(new Program(VS_PATH + "src/shaders/shader.vert", VS_PATH + "src/shaders/shaderTexture.frag"));
		_program.push_back(new Program(VS_PATH + "src/shaders/shader.vert", VS_PATH + "src/shaders/shaderColor.frag"));
	}
	catch (std::exception& e) {}
	if (_program.size() == 0)
	{ 
		std::cout << "Failed to load shaders." << std::endl;
		std::cout << "Are you in the right directory?" << std::endl;
		exit (1);
	}
}

void Application::inputManagerInit()
{
	_input = Input(&_selectedObject);
	_input.setCamera(_camera);
	_input.setWindow(_window);
	_input.initCallbacks();
}

void Application::uiInit()
{
	_ui.setCamera(_camera);
	_ui.setObject(&_selectedObject);
	_ui.setObjectList(&_objects);
	_ui.setTexturesList(&_textures);
	_ui.setObjectToInstantiate(&_objectToInstantiate);

	TextureLoader* texture1 = NULL;
	TextureLoader* texture2 = NULL;
	try {
		texture1 = new TextureLoader(ROOT_PATH + "textures/mouse.png");
		texture2 = new TextureLoader(ROOT_PATH + "textures/mouse2.png");
	}
	catch (std::exception& e) {}
	try {
		texture1 = new TextureLoader(VS_PATH + "textures/mouse.png");
		texture2 = new TextureLoader(VS_PATH + "textures/mouse2.png");
	}
	catch (std::exception& e) {}

	if (texture1 == NULL || texture2 == NULL)
	{
		std::cout << "Failed to load ui texture." << std::endl;
		exit (1);
	}

	_textures.push_back(texture1);
	_textures.push_back(texture2);
	_ui.setTexture(texture1, 0);
	_ui.setTexture(texture2, 1);
}

void Application::backgroundGridInit()
{
	_backgroundGrid = new Grid();
	_backgroundGrid->setProgram(_program[COLOR_SHADER]);
	_backgroundGrid->setProgram(_program[TEXTURE_SHADER]);

	TextureLoader* texture = NULL;
	try {
		texture = new TextureLoader(ROOT_PATH + "textures/grid.png");
	}
	catch (std::exception& e) {}
	try {
		texture = new TextureLoader(VS_PATH + "textures/grid.png");
	}
	catch (std::exception& e) {}

	if (texture == NULL)
	{
		std::cout << "Failed to load background grid texture." << std::endl;
		exit (1);
	}

	_textures.push_back(texture);
	_backgroundGrid->setTexture(texture);

	if (_backgroundGrid == NULL)
	{
		std::cout << "Failed to load background grid." << std::endl;
		exit (1);
	}
}

void Application::loadDefaultObject()
{
	Object* obj = NULL;
	try {
		obj = new Object(ObjectLoader(ROOT_PATH + "objs/Bunny.obj"));
	}
	catch (std::exception& e) {}
	try {
		obj = new Object(ObjectLoader(VS_PATH + "objs/Bunny.obj"));
	}
	catch (std::exception& e) {}
	if (obj)
	{
		obj->addProgram(_program[COLOR_SHADER]);
		obj->addProgram(_program[TEXTURE_SHADER]);
		_objects.push_back(obj);
	}
}

void Application::Loop()
{
	_selectedObject = _objects[0];
	_selectedObject = NULL;

	while (!glfwWindowShouldClose(_window) && glfwGetKey(_window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
	{
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Update section
		{
			if (_objectToInstantiate.empty() == false)
				instantiateObject();

			_input.processInput();

			setViewAndProjectionMatrix(*_camera, *_program[TEXTURE_SHADER]);
			setViewAndProjectionMatrix(*_camera, *_program[COLOR_SHADER]);
			
			_ui.createNewFrame();
			_ui.update();
		}

		// Render section
		{
			for (int i = 0; i < _objects.size(); i++)
				_objects[i]->render();
			_backgroundGrid->render();

			_ui.renderUI();
		}

		glfwSwapBuffers(_window);
		glfwPollEvents();
	}

	_ui.shutdown();

	glfwTerminate();
}

void Application::instantiateObject()
{
	std::string logMessage;

	time_t rawTime;
	struct tm * timeInfo;
	time (&rawTime);
	timeInfo = localtime(&rawTime);
	std::string time(asctime(timeInfo));
	logMessage += time.substr(11, 9); // prints time in format "hh:mm:ss " with no newline

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

	_ui.addToLogs(logMessage);
	_objectToInstantiate.clear();
}


