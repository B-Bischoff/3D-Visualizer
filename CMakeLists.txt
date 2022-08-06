cmake_minimum_required(VERSION 3.18.4)
project(3D-visualizer)

find_package(OpenGL REQUIRED)

add_subdirectory (external)

include_directories(
	external/glfw-3.3.8/include
	external/glm-0.9.9.8/
	external/glew-2.2.0/include
)

set(ALL_LIBS
	${OPENGL_LIBRARY}
	glfw
	GLEW_220
)

add_executable(3D-Visualizer
	src/Application.cpp
	src/Application.hpp
	src/Camera.cpp
	src/Camera.hpp
	src/Grid.cpp
	src/Grid.hpp
	src/Input.cpp
	src/Input.hpp
	src/main.cpp
	src/Object.cpp
	src/Object.hpp
	src/ObjectLoader.cpp
	src/ObjectLoader.hpp
	src/Program.cpp
	src/Program.hpp
	src/TextureLoader.cpp
	src/TextureLoader.hpp
	src/UserInterface.cpp
	src/UserInterface.hpp
	src/vendor/stb/stb_image.h

	src/vendor/imgui/imconfig.h
	src/vendor/imgui/imgui.h
	src/vendor/imgui/imgui.cpp
	src/vendor/imgui/imgui_draw.cpp
	src/vendor/imgui/imgui_impl_glfw.cpp
	src/vendor/imgui/imgui_impl_glfw.h
	src/vendor/imgui/imgui_impl_opengl3.cpp
	src/vendor/imgui/imgui_impl_opengl3.h
	src/vendor/imgui/imgui_impl_opengl3_loader.h
	src/vendor/imgui/imgui_internal.h
	src/vendor/imgui/imgui_tables.cpp
	src/vendor/imgui/imgui_widgets.cpp

	src/vendor/stb/stb_image.h
)
target_link_libraries(3D-Visualizer
	${ALL_LIBS}
)
