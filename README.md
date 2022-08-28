# 3D-Visualizer
This 3D interactive applications allows you to load and visualize ".obj" files content.
You also have the possibility to change objects properties and even apply textures to them.
![demo-01](https://user-images.githubusercontent.com/79707661/187076055-c2d550ef-ac46-4d9c-8461-3c2d6dfa0d6d.png)

## Windows building
Using Visual Studio you can execute the "CMakeLists.txt" script to build and start the application.

## Linux building
Make sure CMake, Git and GCC are installed by typing `sudo apt-get install cmake git g++`
You may also need to install the required packages: `sudo apt-get install libsoil-dev libglm-dev libglew-dev libglfw3-dev libxinerama-dev libxcuros-dev libxi-dev libfreetype-dev libgl1-mesa-dev xorg-dev freeglut3-dev`

To install, build and start the application, type the following commands:

`git clone git@github.com:B-Bischoff/3D-Visualizer.git && cd 3D-Visualizer`

`mkdir build`

`cmake -S . -B build`

`make -C build`

`./build/3D-Visualizer`
