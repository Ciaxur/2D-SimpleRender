#version 320 es
/*
 * Vertex Shader is responsible for the Location
 *  of objects
 */


/* Incomming Data */
layout (location = 0) in vec3 aPos;			// Postion of Variable from Location 0
in vec4 aRGBA;								// Vector4 of <r,g,b,a>

/* Outbound Data */
out vec4 vertexColor;						// Vector of Color outputing to Fragment Shader



void main() {
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
//	vertexColor = aRGBA;
	vertexColor = vec4(0.5, 0.5, 0.0, 1.0);
}