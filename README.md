# OpenGL-library
This is a small OpenGL library with sets RAII wrappers around OpenGL objects such as buffers, windows, vertex arrays, textures... enum classes for elimination of GL_WRONG_ENUM errors and documentation of enums. 

It should eventually become a bare bones game engine with OpenGL-like programming. So still having access to VAOs, VBOs, ... with some abstractions such as mesh and model classes.

This library currently supports basic model loading for the glb (GLTF 2.0) format and obj format.

# How to use the library
This library is built using cmake and gives a template program setup with **C++23** and already linked with the library. To compile the program you simply need these commands in the terminal at the root of your depository:

**PLEASE NOTE THAT THE FOLLOWING COMMANDS ARE TO BUILD WITH MINGW, IF YOU ARE USING OTHER GENERATORS TYPE ```cmake -G --help``` AND CHOOSE THE APPROPRIATE GENERATOR**
```
cmake -S . -B build
cmake --build build
./build/myproject.exe
```
Right now it has only been used with MinGW (GCC) but should work with MSVC according to previous testings.

**If you want to have a debug context, replace**
```
cmake -S . -B build
```
**with**
```
cmake -S . -B build -D CMAKE_CXX_FLAGS="-D MYLIB_DEBUG"
```

# Dependencies
**GLAD** for opengl functions.

**GLFW** for windowing and IO systems.

**GLM** for math.

**STB** for image reading/writing.

**ASSIMP** for model loading.
