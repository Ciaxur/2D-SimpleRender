#include "Shader.h"

#include <spdlog/spdlog.h>
#include <fstream>

/*
 ***********************************************************
 * Shader Structure
 *
 *	- Ease of Use for Shaders
 *  - Compiles and Stores Shader Attributes
 ***********************************************************
 */

void Shader::use() {
  if (status)
    glUseProgram(ID);  // Make Sure ther IS a Valid Program ID
  else
    spdlog::error("Shader Struct: No Program to use!");
}


/**
 ***********************************************************
 * Private Static Methods Backend
 * Initialization of Backend Functionallity
 *		-Shaders
 *		-Buffers
 ***********************************************************
 * Error: Returns 0 if Shader Failed
 */

GLuint loadShader(std::string srcFile, GLenum shaderType) {
  // Load in Source Code
  std::ifstream in(srcFile);
  if (!in.is_open())
    spdlog::error("Shader Initialize: Source Code {} could not be loaded", srcFile.c_str());

  std::string vertSrc((
    std::istreambuf_iterator<char>(in)),
    std::istreambuf_iterator<char>()
  );
  in.close();

  const char *c_str = vertSrc.c_str();

  // Compile and Store Shader
  GLuint shaderID = glCreateShader(shaderType);  // Stores Reference ID
  glShaderSource(shaderID, 1, &c_str, NULL);
  glCompileShader(shaderID);


  // Check for Errors
  int success;
  char infoLog[512];
  glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
    spdlog::error("Initialize Shaders: Error in Compiling Shader Source!\n %s", infoLog);
    return -1;
  }

  // Return the Shader Reference ID Created
  return shaderID;
}

void Shader::compile(const char *vertFilePath, const char *fragFilePath) {
  // Initialize the Shaders
  GLuint fragShader = loadShader(fragFilePath, GL_FRAGMENT_SHADER);
  GLuint vertShader = loadShader(vertFilePath, GL_VERTEX_SHADER);

  // Make sure Fragment Shaders Compiled Correctly
  // Attach & Link Shaders
  if (vertShader != 0 && fragShader != 0) {
    ID = glCreateProgram();

    glAttachShader(ID, vertShader);
    glAttachShader(ID, fragShader);
    glLinkProgram(ID);

    // Check for Linking Errors
    char infoLog[512];
    int success;
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(ID, 512, NULL, infoLog);
      spdlog::error("Program Linking ERROR: Failed to link\n %s", infoLog);

      status = false;
    }

    // Success
    else {
      spdlog::info("Program Shader Compiled Successfuly!");
      status = true;

      // Delete Shaders
      glDeleteShader(vertShader);
      glDeleteShader(fragShader);
    }
  }
}