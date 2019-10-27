#version 330 core
/*
 * Vertex Shader is responsible for the Location
 *  of objects
 */


/* Incomming Data */
layout (location = 0) in vec3 aPos;			// Postion of Variable from Location 0
in vec4 aRGBA;								// RGBA Color of Vertex
in vec2 aTextCoord;							// Texture Drawn Coordinate

/* Outbound Data */
out vec4 vertexColor;						// Vector of Color outputing to Fragment Shader
out vec2 textureCoord;						// Texture Coordinates


void main() {
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);

	// Outbound Data
	vertexColor = aRGBA;
	textureCoord = aTextCoord;

}