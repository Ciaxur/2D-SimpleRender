#pragma once

#include <GL/glew.h>
#include <string>

// Structure for Better Shader Handling
struct Shader {
  GLuint ID;      // Store Compiled Shader Program
  bool status;    // Keep track of Shader Status (False = Not Ready | True = Ready)

  Shader() : ID(0), status(false){};              // No Shader Given
  Shader(GLuint _id) : ID(_id), status(true){};   // Initialize Shader to precompiled Program

  void use();                               // Uses Current Program (If any)
  void compile(const char*, const char*);   // Compiles Given Shader Files (Vertex, Fragment)
};

/**
 * Initializes Source Code of given shaderType
 * @param srcFile - The Source Code path for the Shader
 * @param shaderType - The Shader Type
 * @return - Shader Reference ID, Returns -1 if Failed
 */
GLuint loadShader(std::string srcFile, GLenum shaderType);