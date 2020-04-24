# 2D-SimpleRender
Simple and easy to use 2D OpenGL Engine.


### Linux Setup (GCC and MAKE)
All that is required is **`GLFW3`** and **`GLEW`** libraries to be installed using your packages manager.
Once that is taken care of, building the required files by using the **`make`** command.
- **make build** : Compiles the Source Files and outputs binary as *app*
- **build-prod**: Compiles Source Files with Optimization
- **make build-debug**: Compiles the Source Files for the use of debugging
- **make build-fast**: Compiles the Source Files using Statically Compiled files
- **make build-static**: Statically Compiles the Source Files with Optimization
- **make clean**: Cleans up all build files



### Windows Setup (Visual Studio 2019)
Solution files are found within the **`MS Studio`** directory. The only files required are the **`GLFW3`** and **`GLEW`** binaries.

Obtaining the Binaries,
- Download the [**GLEW Binaries**](http://glew.sourceforge.net/) for Windows 64-bit
    - Must be placed under the following path **`dependencies\glew-2.1.0`**
- Download the [**GLFW3 Binaries**](https://www.glfw.org/download.html) for Windows 64-bit
    - Must be placed under the following path **`dependencies\glfw-3.3`**
