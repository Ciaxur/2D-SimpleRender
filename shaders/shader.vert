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

/**
 * Normalizes a given value from one range to another range.
 *
 * @param v Value with old range.
 * @param oldMin Old minimum range.
 * @param oldMax Old maximum range.
 * @param newMin New minimum range.
 * @param newMax New maximum range.
 *
 * @returns Old value mapped to the new range.
*/
float normalizeFloat(const float v, const float oldMin, const float oldMax, const float newMin, const float newMax) {
  // If both minimum ranges are the same.
  // if ((oldMin - newMin) == 0) return oldMin;

  /*
                      (value−oldMin)×(newMax−newMin)
    normalizedValue =  ------------------------------  + newMin
                                oldMax−oldMin
  */
  float oldRange = oldMax - oldMin;
  float newRange = newMax - newMin;
  return (((v - oldMin) * newRange) / oldRange) + newMin;
}

void main() {
  vec3 pos = vec3(
    normalizeFloat( aPos.x, 0.f, u_res.x, -1.f, 1.f ),
    normalizeFloat( aPos.y, 0.f, u_res.y, -1.f, 1.f ),
    aPos.z
  );

  gl_Position = transform * vec4(pos, 1.0f);

  // Outbound Data
  vertexColor = aRGBA;
  textureCoord = aTextCoord;
}