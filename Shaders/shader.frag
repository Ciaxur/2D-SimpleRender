#version 460 core
/*
 * Fragment Shader is responsible for the color
 *  output for each pixel
 */

/* Shader Settings */
precision mediump float;    // Set Percition to Medium

/* Incomming Data */
in vec4 vertexColor;		// Color from Vertex -> Given to Fragment to Apply
in vec2 textureCoord;		// Texture Coordinates

/* Outbound Data */
out vec4 FragColor;			// Color of Object -> Apply


/* Uniform Data */
uniform sampler2D textureSampler;


void main() {
	FragColor = texture(textureSampler, textureCoord) * vertexColor;

	// Use RGBA Color
//    FragColor = vertexColor;
}