
class vec2 {
   private:
    int x, y;

   public:
    /* Constructors */
    vec2();
    vec2(int);
    vec2(int, int);

    /* Arithmetic on Current Object */
    void add(vec2 &);
    void sub(vec2 &);
    void mult(vec2 &);
    void div(vec2 &);

    /* Useful Object Methods */
    vec2 deepCopy();


    /* Arithmetic Returning a new Object */
    static vec2 add(vec2 &, vec2 &);
    static vec2 sub(vec2 &, vec2 &);
    static vec2 mult(vec2 &, vec2 &);
    static vec2 div(vec2 &, vec2 &);
};