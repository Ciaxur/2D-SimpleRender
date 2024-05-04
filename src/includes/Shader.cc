#include "Shader.h"

#include <fstream>
#include <spdlog/spdlog.h>
#include <sys/stat.h>


Shader::Shader() : ID(0), ready(false) {};              // No Shader Given
Shader::Shader(GLuint _id) : ID(_id), ready(true) {};   // Initialize Shader to precompiled Program
Shader::~Shader() { this->deleteShader(); }

/*
 ***********************************************************
 * Shader Structure
 *
 *	- Ease of Use for Shaders
 *  - Compiles and Stores Shader Attributes
 ***********************************************************
 */

void Shader::use() {
  if (ready)
    glUseProgram(ID);  // Make Sure ther IS a Valid Program ID
  else
    spdlog::error("Shader Struct: No Program to use!");
}

/**
 ***********************************************************
 * Loads a shader type from a file.
 *		-Shaders
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
    spdlog::error("Initialize Shaders: Error in Compiling Shader Source!\n {}", infoLog);
    return -1;
  }

  // Return the Shader Reference ID Created
  return shaderID;
}

void Shader::compile(const char *vertFilePath, const char *fragFilePath) {
  // Store a copy of the shader file paths.
  this->fragmentShaderFilepath = fragFilePath;
  this->vertexShaderFilepath = vertFilePath;

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
      spdlog::error("Program Linking ERROR: Failed to link\n {}", infoLog);

      ready = false;
    }

    // Success
    else {
      spdlog::info("Program Shader Compiled Successfuly!");
      ready = true;

      // Delete Shaders
      glDeleteShader(vertShader);
      glDeleteShader(fragShader);
    }
  }
}

void Shader::deleteShader() {
  if (this->ID != 0) {
    glDeleteProgram(this->ID);
  }
}

/**
 ***********************************************************
 * Live re-loads the shader if the file was modified.
 *		- Shaders
 ***********************************************************
 * Error: Returns 0 if Shader Failed
 */

/* Obtains last updated time of given file */
static time_t getLastModified(const char* filename) {
  struct stat st;
  if (stat(filename, &st) == 0) {
    return st.st_mtime;
  }

  return -1;
}

/* Updates Shaders Live */
void Shader::liveGLSLUpdateShaders() {
  // Early return if no shaders have been compiled yet.
  if (!this->ready || this->fragmentShaderFilepath.empty() || this->vertexShaderFilepath.empty()) return;

  // Check & Update Fragment Shader
  time_t newFshaderLastMod = getLastModified(this->fragmentShaderFilepath.c_str());
  time_t newVshaderLastMod = getLastModified(this->vertexShaderFilepath.c_str());

  if (newFshaderLastMod != FshaderLastMod || newVshaderLastMod != VshaderLastMod) {
    FshaderLastMod = newFshaderLastMod;
    VshaderLastMod = newVshaderLastMod;

    // Re-Compile Shaders
    // Attach & Link Shaders & Use
    this->compile(this->vertexShaderFilepath.c_str(), this->fragmentShaderFilepath.c_str());
  }
}