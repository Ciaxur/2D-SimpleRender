#include "Vector_D.h"

/* Creates a Default Vector2D */
vec2::vec2() { x = y = 0; }

/**
 * Initializes Vector2D with an x Attribute
 *
 * @param _x - The x Value of Vector2D
 */
vec2::vec2(float _x) {
    x = _x;
    y = 0;
}

/**
 * Initializes Vector2D with both Attributes
 *
 * @param _x - The x Value of Vector2D
 * @param _y - The y Value of Vector2D
 */
vec2::vec2(float _x, float _y) {
    x = _x;
    y = _y;
}


/**
 * Adds current Vector2D by another Vector2D
 *
 * @param other - Reference to another Vector2D Instance
 */
void vec2::add(vecX& other) {
    this->x += other.x;
    this->y += other.y;
}

/**
 * Subtracts current Vector2D from another Vector2D
 *
 * @param other - Reference to another Vector2D Instance
 */
void vec2::sub(vecX& other) {
    this->x -= other.x;
    this->y -= other.y;
}

/**
 * Multiplies current Vector2D by another Vector2D
 *
 * @param other - Reference to another Vector2D Instance
 */
void vec2::mult(vecX& other) {
    this->x *= other.x;
    this->y *= other.y;
}

/**
 * Divides current Vector2D by another Vector2D
 *
 * @param other - Reference to another Vector2D Instance
 */
void vec2::div(vecX& other) {
    this->x /= other.x;
    this->y /= other.y;
}

/**
 * Creates a Deep Copy of Currenct Vector2D
 *
 * @returns New Deep Copy of Currenct Vector2D
 */
vec2 vec2::deepCopy() {
    vec2 newV(this->x, this->y);
    return newV;
}


/**
 * Adds current Vector2D with another Vector2D
 *  returning the result in a new Vector2D Instance
 *
 * @param v1 - Reference to first Vector2D Instance
 * @param v2 - Reference to first Vector2D Instance
 * @returns - New Vector2D with result of both given Instances
 */
vec2 vec2::add(vec2& v1, vec2& v2) {
    vec2 v3(v1.x + v2.x, v1.y + v2.y);
    return v3;
}

/**
 * Subtracts current Vector2D with another Vector2D
 *  returning the result in a new Vector2D Instance
 *
 * @param v1 - Reference to first Vector2D Instance
 * @param v2 - Reference to first Vector2D Instance
 * @returns - New Vector2D with result of both given Instances
 */
vec2 vec2::sub(vec2& v1, vec2& v2) {
    vec2 v3(v1.x - v2.x, v1.y - v2.y);
    return v3;
}

/**
 * Multiplies current Vector2D with another Vector2D
 *  returning the result in a new Vector2D Instance
 *
 * @param v1 - Reference to first Vector2D Instance
 * @param v2 - Reference to first Vector2D Instance
 * @returns - New Vector2D with result of both given Instances
 */
vec2 vec2::mult(vec2& v1, vec2& v2) {
    vec2 v3(v1.x * v2.x, v1.y * v2.y);
    return v3;
}

/**
 * Divides current Vector2D with another Vector2D
 *  returning the result in a new Vector2D Instance
 *
 * @param v1 - Reference to first Vector2D Instance
 * @param v2 - Reference to first Vector2D Instance
 * @returns - New Vector2D with result of both given Instances
 */
vec2 vec2::div(vec2& v1, vec2& v2) {
    vec2 v3(v1.x / v2.x, v1.y / v2.y);
    return v3;
}
