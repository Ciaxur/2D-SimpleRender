
/**
 * N-Dimensional Template for Vector Classes
 */
class vecX {
   public:
    float x = 0, y = 0, z = 0, w = 0;

   public:
    virtual void add(vecX &) = 0;
    virtual void sub(vecX &) = 0;
    virtual void mult(vecX &) = 0;
    virtual void div(vecX &) = 0;
};


/**
 * 2D Vector Class
 *  - Manipulating and Keeping track of
 *      2D Vectors
 */
class vec2 : public vecX {
   public:
    /* Constructors */
    vec2();
    vec2(float);
    vec2(float, float);

    /* Arithmetic on Current Object */
    void add(vecX &);
    void sub(vecX &);
    void mult(vecX &);
    void div(vecX &);

    /* Useful Object Methods */
    vec2 deepCopy();


    /* Arithmetic Returning a new Object */
    static vec2 add(vec2 &, vec2 &);
    static vec2 sub(vec2 &, vec2 &);
    static vec2 mult(vec2 &, vec2 &);
    static vec2 div(vec2 &, vec2 &);
};


/**
 * 3D Vector Class
 *  - Manipulating and Keeping track of
 *      3D Vectors
 */
class vec3 : public vecX {
   public:
    /* Constructors */
    vec3();
    vec3(float);
    vec3(float, float);
    vec3(float, float, float);

    /* Arithmetic on Current Object */
    void add(vecX &);
    void sub(vecX &);
    void mult(vecX &);
    void div(vecX &);

    /* Useful Object Methods */
    vec3 deepCopy();


    /* Arithmetic Returning a new Object */
    static vec3 add(vec3 &, vec3 &);
    static vec3 sub(vec3 &, vec3 &);
    static vec3 mult(vec3 &, vec3 &);
    static vec3 div(vec3 &, vec3 &);
};