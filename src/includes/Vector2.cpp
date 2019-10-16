#include "Vector.h"

/* Creates a Default Vector2D */
Vector2::Vector2() { x = y = 0; }

/**
 * Initializes Vector2D with an x Attribute
 *
 * @param _x - The x Value of Vector2D
 */
Vector2::Vector2(float _x) {
    x = _x;
    y = 0;
}

/**
 * Initializes Vector2D with both Attributes
 *
 * @param _x - The x Value of Vector2D
 * @param _y - The y Value of Vector2D
 */
Vector2::Vector2(float _x, float _y) {
    x = _x;
    y = _y;
}


/**
 * Adds current Vector2D by another Vector2D
 *
 * @param other - Reference to another Vector2D Instance
 */
void Vector2::add(Vector &other) {
    this->x += other.x;
    this->y += other.y;
}

/**
 * Subtracts current Vector2D from another Vector2D
 *
 * @param other - Reference to another Vector2D Instance
 */
void Vector2::sub(Vector &other) {
    this->x -= other.x;
    this->y -= other.y;
}

/**
 * Multiplies current Vector2D by another Vector2D
 *
 * @param other - Reference to another Vector2D Instance
 */
void Vector2::mult(Vector &other) {
    this->x *= other.x;
    this->y *= other.y;
}

/**
 * Divides current Vector2D by another Vector2D
 *
 * @param other - Reference to another Vector2D Instance
 */
void Vector2::div(Vector &other) {
    this->x /= other.x;
    this->y /= other.y;
}

/**
 * Creates a Deep Copy of Currenct Vector2D
 *
 * @returns New Deep Copy of Currenct Vector2D
 */
Vector2 Vector2::deepCopy() {
    Vector2 newV(this->x, this->y);
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
Vector2 Vector2::add(Vector2 &v1, Vector2 &v2) {
    Vector2 v3(v1.x + v2.x, v1.y + v2.y);
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
Vector2 Vector2::sub(Vector2 &v1, Vector2 &v2) {
    Vector2 v3(v1.x - v2.x, v1.y - v2.y);
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
Vector2 Vector2::mult(Vector2 &v1, Vector2 &v2) {
    Vector2 v3(v1.x * v2.x, v1.y * v2.y);
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
Vector2 Vector2::div(Vector2 &v1, Vector2 &v2) {
    Vector2 v3(v1.x / v2.x, v1.y / v2.y);
    return v3;
}



/* HELPER FUNCTIONS */

/**
 * Calculates the Raw Size of a given Vector32
 *
 * @param vec - Vector2 Vector
 * @returns Raw Size of Vector
 */
size_t getVector2Size(std::vector<Vector2> vec) {
	return (vec.size() * 2) * sizeof(float);
}


/**
 * Returns Raw Float Data from Vector2 Vector
 *
 * @param vec - Vector of Vector2 Objects
 * @param out - Pointer to Array of Floats
 */
void getRawVecData(std::vector<Vector2> vec, float* out) {
	int i = 0;
	for (Vector2& v : vec) {
		out[i++] = v.x;
		out[i++] = v.y;
	}
}