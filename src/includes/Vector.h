#ifndef Vector_H
#define Vector_H

// Core Libraries
#include <vector>
#include <cstdio>



/**
 * N-Dimensional Template for Vector Classes
 */
class Vector {
  public:
    float x = 0, y = 0, z = 0, w = 0;

  public:
    virtual void add	(Vector &)	= 0;
    virtual void sub	(Vector &)	= 0;
    virtual void mult	(Vector &)	= 0;
    virtual void div	(Vector &)	= 0;
};


/**
 * 2D Vector Class
 *  - Manipulating and Keeping track of
 *      2D Vectors
 */
class Vector2 : public Vector {
  public:
    /* Constructors */
    Vector2();
    Vector2(float);
    Vector2(float, float);

    /* Arithmetic on Current Object */
    void add	(Vector &);
    void sub	(Vector &);
    void mult	(Vector &);
    void div	(Vector &);

    /* Useful Object Methods */
	Vector2 deepCopy();


    /* Arithmetic Returning a new Object */
    static Vector2 add	(Vector2 &, Vector2 &);
    static Vector2 sub	(Vector2 &, Vector2 &);
    static Vector2 mult	(Vector2 &, Vector2 &);
    static Vector2 div	(Vector2 &, Vector2 &);
};


/**
 * 3D Vector Class
 *  - Manipulating and Keeping track of
 *      3D Vectors
 */
class Vector3 : public Vector {
  public:
    /* Constructors */
    Vector3();
    Vector3(float);
    Vector3(float, float);
    Vector3(float, float, float);

    /* Arithmetic on Current Object */
    void add	(Vector &);
    void sub	(Vector &);
    void mult	(Vector &);
    void div	(Vector &);

    /* Useful Object Methods */
	Vector3 deepCopy();


    /* Arithmetic Returning a new Object */
    static Vector3 add	(Vector3 &, Vector3 &);
    static Vector3 sub	(Vector3 &, Vector3 &);
    static Vector3 mult	(Vector3 &, Vector3 &);
    static Vector3 div	(Vector3 &, Vector3 &);
};



/* Helper Functions */
void printVector(Vector &);
size_t getVec3Size(std::vector<Vector3>);
size_t getVec2Size(std::vector<Vector2>);
void getRawVecData(std::vector<Vector2> vec, float* out);
void getRawVecData(std::vector<Vector3> vec, float* out);

#endif // !Vector_H
