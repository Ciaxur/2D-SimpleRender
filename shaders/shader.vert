#version 460 core
/*
 * Vertex Shader is responsible for the location of objects
 */


/* Incomming Data */
layout (location = 0) in vec3 aPos;		// Position of Variable from Location 0
in vec4 aRGBA;												// RGBA Color of Vertex
in vec2 aTextCoord;										// Texture Drawn Coordinate

/* Outbound Data */
out vec4 vertexColor;						// Vector of Color outputing to Fragment Shader
out vec2 textureCoord;					// Texture Coordinates

/* Uniform Data */
uniform mat4 transform;
uniform vec2 u_res;

void main() {
  gl_Position = transform * vec4(aPos, 1.0f);

  // Outbound Data
  vertexColor = aRGBA;
  textureCoord = aTextCoord;
}