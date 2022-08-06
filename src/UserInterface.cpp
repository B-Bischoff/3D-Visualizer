#include "UserInterface.hpp"
#include "imgui/imgui.h"

UserInterface::UserInterface(const int winWidth, const int winHeight)
	: WIN_WIDTH(winWidth), WIN_HEIGHT(winHeight)
{
}

void UserInterface::update()
{
	updateObjetMenu();
	updateHierarchy();
	updateCamera();
	updateInstantiate();
}

void UserInterface::updateObjetMenu()
{
	Object* obj = *_object;

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	ImGui::SetNextWindowPos(ImVec2(WIN_WIDTH - 400, 0), 0);
	ImGui::SetNextWindowSize(ImVec2(400, WIN_HEIGHT / 2.0f), 0);
	ImGuiWindowFlags windowFlag = 0;
	windowFlag |= ImGuiWindowFlags_NoMove;
	windowFlag |= ImGuiWindowFlags_NoCollapse;
	windowFlag |= ImGuiWindowFlags_NoResize;

	ImGui::Begin("Model properties:");
	ImGui::Text("Position (in world unit)");
	ImGui::DragFloat("X position", &obj->translation.x, 0.05f);
	ImGui::DragFloat("Y position", &obj->translation.y, 0.01f);
	ImGui::DragFloat("Z position", &obj->translation.z, 0.01f);

	ImGui::Text("Rotation (in degree)");
	ImGui::DragFloat("X rotation", &obj->rotation.x, 0.5f);
	ImGui::DragFloat("Y rotation", &obj->rotation.y, 0.5f);
	ImGui::DragFloat("Z rotation", &obj->rotation.z, 0.5f);

	ImGui::Text("Scale");
	ImGui::DragFloat("X scale", &obj->scale.x, 0.01f);
	ImGui::DragFloat("Y scale", &obj->scale.y, 0.01f);
	ImGui::DragFloat("Z scale", &obj->scale.z, 0.01f);

	ImGui::Text("Drawing mode");
	int renderModeCombo = obj->renderMode;
	ImGui::Combo("Selection", &renderModeCombo, "Triangle\0Wireframe\0points");
	obj->renderMode = Drawing_mode(renderModeCombo);

	ImGui::Text("Material type");
	int materialTypeCombo = obj->materialType;
	ImGui::Combo("combo 2", &materialTypeCombo, "Color\0Texture");
	obj->materialType = Material_type(materialTypeCombo);

	if (obj->materialType == Material_type::COLOR)
	{
		ImGui::Text("Object color");
		static ImVec4 color = ImVec4(obj->color.r, obj->color.g, obj->color.b, obj->color.t);
		ImGui::PushItemWidth(150);
		ImGui::ColorPicker3("##MyColor##5", (float*)&color, ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha);
		ImGui::SameLine();
		if (ImGui::Button("Random colors"))
			obj->applyRandomColors();
		ImGui::SameLine();
		if (ImGui::Button("Colors from position"))
			obj->applyColorsFromPositions();


		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		glm::vec4 newColor = { color.x, color.y, color.z, color.w };
		if (obj->color != newColor)
		{
			obj->color = newColor;
			obj->applyColor();
		}
	}
	else if (obj->materialType == Material_type::TEXTURE)
	{
		ImGui::Text("Available textures: ");
		int textureSelected = -1;
		for (int i = 0; i < _textureList->size(); i++)
		{
			char buf[32];
			sprintf_s(buf, "Texture %d", i);
			if (ImGui::Selectable(buf, textureSelected == i))
				textureSelected = i;
		}

		if (textureSelected != -1)
			(*_object)->setTexture((*_textureList)[textureSelected]);
	}


	ImGui::End();
}

void UserInterface::updateHierarchy()
{
	ImGui::SetNextWindowPos(ImVec2(0, 0), 0);
	ImGuiWindowFlags windowFlag = 0;
	windowFlag |= ImGuiWindowFlags_NoMove;


	ImGui::Begin("Hierarchy:", NULL, windowFlag);

	static int hierarchySelected = -1;
	for (int i = 0; i < _objectList->size(); i++)
	{
		char buf[32];
		Object* obj = (*_objectList)[i];
	
		sprintf_s(buf, "Show/Hide %d", i);
		ImGui::Checkbox(buf, &obj->visible);

		ImGui::SameLine();
		
		sprintf_s(buf, "Object %d", i);
		if (ImGui::Selectable(buf, hierarchySelected == i))
			hierarchySelected = i;
	}

	if (hierarchySelected != -1)
		*_object = (*_objectList)[hierarchySelected];

	ImGui::End();
}

void UserInterface::updateCamera()
{
	ImGui::SetNextWindowPos(ImVec2(WIN_WIDTH - 400, WIN_HEIGHT / 2.0f), 0);
	ImGui::SetNextWindowSize(ImVec2(400, WIN_HEIGHT / 2.0f), 0);
	ImGuiWindowFlags windowFlag = 0;
	windowFlag |= ImGuiWindowFlags_NoMove;
	windowFlag |= ImGuiWindowFlags_NoResize;

	ImGui::Begin("Camera:", NULL, windowFlag);

	ImGui::Text("Position (in world unit)");
	ImGui::DragFloat("X position", &_camera->position.x, 0.05f);
	ImGui::DragFloat("Y position", &_camera->position.y, 0.01f);
	ImGui::DragFloat("Z position", &_camera->position.z, 0.01f);

	ImGui::Text("Pivot point");
	ImGui::DragFloat("Pivot X position", &_camera->targetPos.x, 0.05f);
	ImGui::DragFloat("Pivot Y position", &_camera->targetPos.y, 0.01f);
	ImGui::DragFloat("Pivot Z position", &_camera->targetPos.z, 0.01f);

	ImGui::Text("Make selected object as pivot: ");
	ImGui::SameLine();
	if (ImGui::Button("Update"))
		_camera->targetPos = (*_object)->translation;

	ImGui::Text("Camera sensitivity");
	
	ImGui::InputFloat("Zoom sensitivity", &_camera->zoomSpeed, 0.01f, 1.0f, "%.2f");
	ImGui::InputFloat("Translation speed", &_camera->translationSpeed, 0.01f, 1.0f, "%.2f");
	ImGui::InputFloat("Rotation speed", &_camera->rotationSpeed, 0.01f, 1.0f, "%.2f");

	ImGui::End();
}

void UserInterface::updateInstantiate()
{
	ImGui::SetNextWindowPos(ImVec2(0, WIN_HEIGHT - 400), 0);
	ImGui::SetNextWindowSize(ImVec2(600, 400), 0);
	ImGuiWindowFlags windowFlag = 0;
	windowFlag |= ImGuiWindowFlags_NoMove;

	ImGui::Begin("Load files:", NULL, windowFlag);

	ImGui::Text("Load .obj or texture file: ");
	bool reclaimFocus = false;
	static char buf1[256] = "";
	ImGui::PushItemWidth(350);
	ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue;
	if (ImGui::InputText("file location (absolute path)", buf1, 256, input_text_flags))
	{
		*_objectToInstantiate = std::string(buf1);
		buf1[0] = '\0';
		reclaimFocus = true;
	}

	ImGui::SetItemDefaultFocus();
	if (reclaimFocus)
		ImGui::SetKeyboardFocusHere(-1);

	if (ImGui::Button("Clear logs"))
		_logs.clear();
	ImGui::Text(_logs.c_str());

	ImGui::End();
}

void UserInterface::addToLogs(std::string& string)
{
	_logs += string;
	_logs += '\n';
}

void UserInterface::createNewFrame() const
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void UserInterface::renderUI() const
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UserInterface::shutdown() const
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void UserInterface::setObject(Object** object)
{
	_object = object;
}

void UserInterface::setCamera(Camera* camera)
{
	_camera = camera;
}

void UserInterface::setObjectList(std::vector<Object*>* objectList)
{
	_objectList = objectList;
}

void UserInterface::setTexturesList(std::vector<TextureLoader*>* textureList)
{
	_textureList = textureList;
}

void UserInterface::setObjectToInstantiate(std::string* string)
{
	this->_objectToInstantiate = string;
}
