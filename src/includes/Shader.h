#pragma once

#include <GL/glew.h>
#include <string>

// Structure for Better Shader Handling
class Shader {
  private:
    // Paths to compiled shaders.
    std::string vertexShaderFilepath;
    std::string fragmentShaderFilepath;
    time_t FshaderLastMod = 0, VshaderLastMod = 0;

  public:
    GLuint ID;      // Store Compiled Shader Program
    bool ready;    // Keep track of Shader Status (False = Not Ready | True = Ready)

  public:
    Shader();               // No Shader Given
    Shader(GLuint _id);     // Initialize Shader to precompiled Program
    ~Shader();

    void use();                               // Uses Current Program (If any)
    void compile(const char*, const char*);   // Compiles Given Shader Files (Vertex, Fragment)
    void liveGLSLUpdateShaders();             // Updates the shader if the filepath was modified.
    void deleteShader();                      // Cleans up shader.
};

/**
 * Initializes Source Code of given shaderType
 * @param srcFile - The Source Code path for the Shader
 * @param shaderType - The Shader Type
 * @return - Shader Reference ID, Returns -1 if Failed
 */
GLuint loadShader(std::string srcFile, GLenum shaderType);