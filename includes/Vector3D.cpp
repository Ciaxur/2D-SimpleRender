#include "Vector_D.h"

/* Creates a Default Vector3D */
vec3::vec3() { x = y = z = 0; }

/**
 * Initializes Vector3D with an x Attribute
 *
 * @param _x - The x Value of Vector3D
 */
vec3::vec3(float _x) {
    x = _x;
    y = 0;
    z = 0;
}

/**
 * Initializes Vector3D with x and y Attributes
 *
 * @param _x - The x Value of Vector3D
 * @param _y - The y Value of Vector3D
 */
vec3::vec3(float _x, float _y) {
    x = _x;
    y = _y;
    z = 0;
}

/**
 * Initializes Vector3D with all Attributes
 *
 * @param _x - The x Value of Vector3D
 * @param _y - The y Value of Vector3D
 * @param _z - The z Value of Vector3D
 */
vec3::vec3(float _x, float _y, float _z) {
    x = _x;
    y = _y;
    z = _z;
}

/**
 * Adds current Vector3D by another Vector3D
 *
 * @param other - Reference to another Vector3D Instance
 */
void vec3::add(vecX& other) {
    this->x += other.x;
    this->y += other.y;
    this->z += other.z;
}

/**
 * Subtracts current Vector3D from another Vector3D
 *
 * @param other - Reference to another Vector3D Instance
 */
void vec3::sub(vecX& other) {
    this->x -= other.x;
    this->y -= other.y;
    this->z -= other.z;
}

/**
 * Multiplies current Vector3D by another Vector3D
 *
 * @param other - Reference to another Vector3D Instance
 */
void vec3::mult(vecX& other) {
    this->x *= other.x;
    this->y *= other.y;
    this->z *= other.z;
}

/**
 * Divides current Vector3D by another Vector3D
 *
 * @param other - Reference to another Vector3D Instance
 */
void vec3::div(vecX& other) {
    this->x /= other.x;
    this->y /= other.y;
    this->z /= other.z;
}

/**
 * Creates a Deep Copy of Currenct Vector3D
 *
 * @returns New Deep Copy of Currenct Vector3D
 */
vec3 vec3::deepCopy() {
    vec3 newV(this->x, this->y, this->z);
    return newV;
}


/**
 * Adds current Vector3D with another Vector3D
 *  returning the result in a new Vector3D Instance
 *
 * @param v1 - Reference to first Vector3D Instance
 * @param v2 - Reference to first Vector3D Instance
 * @returns - New Vector3D with result of both given Instances
 */
vec3 vec3::add(vec3& v1, vec3& v2) {
    vec3 v3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
    return v3;
}

/**
 * Subtracts current Vector3D with another Vector3D
 *  returning the result in a new Vector3D Instance
 *
 * @param v1 - Reference to first Vector3D Instance
 * @param v2 - Reference to first Vector3D Instance
 * @returns - New Vector3D with result of both given Instances
 */
vec3 vec3::sub(vec3& v1, vec3& v2) {
    vec3 v3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
    return v3;
}

/**
 * Multiplies current Vector3D with another Vector3D
 *  returning the result in a new Vector3D Instance
 *
 * @param v1 - Reference to first Vector3D Instance
 * @param v2 - Reference to first Vector3D Instance
 * @returns - New Vector3D with result of both given Instances
 */
vec3 vec3::mult(vec3& v1, vec3& v2) {
    vec3 v3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
    return v3;
}

/**
 * Divides current Vector3D with another Vector3D
 *  returning the result in a new Vector3D Instance
 *
 * @param v1 - Reference to first Vector3D Instance
 * @param v2 - Reference to first Vector3D Instance
 * @returns - New Vector3D with result of both given Instances
 */
vec3 vec3::div(vec3& v1, vec3& v2) {
    vec3 v3(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z);
    return v3;
}
