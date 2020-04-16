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
uniform float u_time;
uniform vec2 u_mouse;
uniform vec2 u_res;



void main() {
	// DEBUG: Testing out Uniform Data
	vec2 st = gl_FragCoord.xy/u_res.xy;
    st.x *= u_res.x/u_res.y;

    vec3 color = vec3(0.0);
    color = vec3(st.x,st.y,abs(sin(u_time)));

	FragColor = vec4(color, 1.0);

	// FragColor = texture(textureSampler, textureCoord) * vertexColor * vec4(color, 1.0);
	

	// Use RGBA Color
//    FragColor = vertexColor;
}