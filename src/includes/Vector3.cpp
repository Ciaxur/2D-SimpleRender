#include "Vector.h"

/* Creates a Default Vector3D */
Vector3::Vector3() { x = y = z = 0; }

/**
 * Initializes Vector3D with an x Attribute
 *
 * @param _x - The x Value of Vector3D
 */
Vector3::Vector3(float _x) {
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
Vector3::Vector3(float _x, float _y) {
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
Vector3::Vector3(float _x, float _y, float _z) {
    x = _x;
    y = _y;
    z = _z;
}

/**
 * Adds current Vector3D by another Vector3D
 *
 * @param other - Reference to another Vector3D Instance
 */
void Vector3::add(Vector &other) {
    this->x += other.x;
    this->y += other.y;
    this->z += other.z;
}

/**
 * Subtracts current Vector3D from another Vector3D
 *
 * @param other - Reference to another Vector3D Instance
 */
void Vector3::sub(Vector &other) {
    this->x -= other.x;
    this->y -= other.y;
    this->z -= other.z;
}

/**
 * Multiplies current Vector3D by another Vector3D
 *
 * @param other - Reference to another Vector3D Instance
 */
void Vector3::mult(Vector &other) {
    this->x *= other.x;
    this->y *= other.y;
    this->z *= other.z;
}

/**
 * Divides current Vector3D by another Vector3D
 *
 * @param other - Reference to another Vector3D Instance
 */
void Vector3::div(Vector &other) {
    this->x /= other.x;
    this->y /= other.y;
    this->z /= other.z;
}

/**
 * Creates a Deep Copy of Currenct Vector3D
 *
 * @returns New Deep Copy of Currenct Vector3D
 */
Vector3 Vector3::deepCopy() {
    Vector3 newV(this->x, this->y, this->z);
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
Vector3 Vector3::add(Vector3 &v1, Vector3 &v2) {
    Vector3 v3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
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
Vector3 Vector3::sub(Vector3 &v1, Vector3 &v2) {
    Vector3 v3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
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
Vector3 Vector3::mult(Vector3 &v1, Vector3 &v2) {
    Vector3 v3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
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
Vector3 Vector3::div(Vector3 &v1, Vector3 &v2) {
    Vector3 v3(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z);
    return v3;
}


/* HELPER FUNCTIONS */


/**
 * Calculates the Raw Size of a given Vector3
 *
 * @param vec - Vector3 Vector
 * @returns Raw Size of Vector
 */
size_t getVec3Size(std::vector<Vector3> vec) {
	return (vec.size() * 3) * sizeof(float);
}

/** 
 * Returns Raw Float Data from Vector3 Vector
 *
 * @param vec - Vector of Vector3 Objects
 * @param out - Pointer to Array of Floats
 */
void getRawVecData(std::vector<Vector3> vec, float* out) {
	int i = 0;
	for (Vector3& v : vec) {
		out[i++] = v.x;
		out[i++] = v.y;
		out[i++] = v.z;
	}
}