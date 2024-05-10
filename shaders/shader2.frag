#version 460 core
/*
 * Fragment Shader is responsible for the color output for each pixel
 */

/* Shader Settings */
precision mediump float;    // Set precision to Medium

/* Incomming Data */
in vec4 vertexColor;		// Color from Vertex -> Given to Fragment to Apply
in vec2 textureCoord;		// Texture Coordinates

/* Outbound Data */
out vec4 FragColor;			// Color of Object -> Apply

/* Uniform Data */
uniform sampler2D textureSampler;
uniform float u_time;
uniform vec2 u_mouse;
uniform vec2 u_res;
uniform bool useTexture;     // Flag to use a texture instead of a solid color.
uniform vec4 solidColor;



void main() {
  // Apply the texture onto the pixel.
  if (useTexture) {
    vec4 tex = texture(textureSampler, textureCoord);
    FragColor = tex;
  }

  // Use solid color.
  else {
    FragColor = solidColor;
  }
}