#version 320 es
/*
 * Fragment Shader is responsible for the color
 *  output for each pixel
 */

precision mediump float;    // Set Percition to Medium
out vec4 FragColor;

void main() {
    FragColor = vec4(0.2f, 0.5f, 0.2f, 1.0f);
}
