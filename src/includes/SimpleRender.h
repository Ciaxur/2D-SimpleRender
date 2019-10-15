#ifndef SimpleRender_H
#define SimpleRender_H

// Core Libraries
#include <cstring>
#include <fstream>
#include <iostream>
#include <string.h>
#include <vector>

// OpenGL Libraries
#include <GL/glew.h>
#include <GLFW/glfw3.h>

/**
 * Stores Data Objects of the There
 *  linked buffers.
 *      - Vertex Array Object       (VAO)
 *          - Buffer the holds the Verticies Array
 *      - Vertex Buffer Object      (VBO)
 *          - Vertex Buffer that holds all the configurations of Vertex Array
 *      - Element Buffer Object     (EBO)
 *          - Buffer that holds the indicies of how to draw Array Data
 *      - Number of Elements Indicies
 *
 */
struct BufferData {
    BufferData(): VAO(0), VBO(0), EBO(0){};
    BufferData(GLuint &_vao, GLuint &_vbo, GLuint &_ebo)
        : VAO(_vao), VBO(_vbo), EBO(_ebo){};

    GLuint VAO;
    GLuint VBO;
    GLuint EBO;

    size_t indiciesElts = 0;
};


class SimpleRender {
  private: // Private Variables | GL Window Data
    GLFWwindow *window;
    GLuint programID;
    const char *title = "GLFW Window";
    const unsigned int WIDTH = 400;
    const unsigned int HEIGHT = 400;

  protected: // Shared Variables
     std::vector<BufferData> bufferData; // Store References the Buffer Data


  private: // Private Methods (Static - Callbacks)
    /* Called when Key Pressed */
    static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

    /* Mouse Click Callback */
    static void mouseBtn_callback(GLFWwindow *window, int button, int action, int mods);

    /* Mouse Cursor Position Relative to Window Callback */
    static void cursorPos_callback(GLFWwindow *window, double xPos, double yPos);

    /* Mouse Cursor Scroll Offset Callback */
    static void mouseScroll_callback(GLFWwindow *window, double xOffset, double yOffset);

    /* GLFW Error Callback */
    static void error_callback(int error, const char *description);

    /**
     * Initializes Source Code of given shaderType
     * @param srcFile - The Source Code path for the Shader
     * @param shaderType - The Shader Type
     * @return - Shader Reference ID, Returns -1 if Failed
     */
    static GLuint InitShader(std::string srcFile, GLenum shaderType);


  protected: // Shared Methods
    /**
     * Creates Buffer data for Verticies & Indicies provided
     *  by creating a VAO linked to a VBO and EBO.
     * Data is configured and packaged in an Object with the
     *  reference IDs given by OpenGL and returned.
     *
     * @param verticies - The Verticies Array, seperated by (x,y,z)/Vertex
     * @param vSize     - Size of the array in Bytes (sizeof(verticies))
     * @param indicies - The Indicies Array, specifying the order of Vertex to be drawn
     * @param iSize     - Size of the array in Bytes (sizeof(indicies))
     * @return BufferData Object with the Object Reference IDs stored
     */
    static BufferData createBuffer(GLfloat *verticies, size_t vSize, GLuint *indicies, size_t iSize);


  private: // Helper Functions
    /**
     * Draw loop
     */
    virtual void Draw();

    /**
     * Data/Properties to configure prior to Drawing
     */
    virtual void Preload();

    /**
     * Handles User Input from given Window
     * @param window - The GLFW Window Reference
     */
    void processInput(GLFWwindow *window);



  public: // Main User-End Operation Functions
    /**
     * Constuctor that initalizes GLFW with given Width and Height
     * @param w - The Width of the Window
     * @param h - The Height of the Window
     */
    SimpleRender(unsigned int w, unsigned int h, const char *title = "GLFW Window");

    /**
     * Destructor to clean up Resources used by OpenGL
     */
    ~SimpleRender();

    /**
     * Constuctor that initalizes GLFW with default Width and Height
     */
    void InitRender();

    /**
     * Starts running OpenGL window
     * @returns - Value '-1' or '0' to determine the exit State
     */
    int run();
};


#endif // !SimpleRender_H
