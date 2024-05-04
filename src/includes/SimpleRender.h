#pragma once

// Core Libraries
#include <spdlog/spdlog.h>
#include <stdio.h>
#include <string.h>

#include <cstring>
#include <fstream>
#include <memory>
#include <iostream>
#include <vector>

// OpenGL Libraries
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/imgui.h>

// GL Math Library
#include <glm/gtc/type_ptr.hpp>  // Memory Access
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

// Project Libraries
#include "BufferData.h"
#include "Shader.h"



class SimpleRender {
  private:  // Private Variables | GL Window Data
    unsigned int WIDTH = 400;
    unsigned int HEIGHT = 400;
    double FPS;      // Current Calculated FPS Value
    glm::vec2 mousePos;  // Current Mouse Position


  protected:  // Shared Window Data
    GLFWwindow* window;
    const char* title = "GLFW Window";
    char titleBuffer[256];  // Used for Temporary Character Storage (Window Title)


  protected:  // Shared Variables
    std::shared_ptr<Shader> defaultShader;
    std::vector<BufferData> bufferData;  // Store References the Buffer Data



  private:  // Private Methods (Static - Callbacks)
    /* Called when Key Pressed */
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

    /* Mouse Click Callback */
    static void mouseBtn_callback(GLFWwindow* window, int button, int action, int mods);

    /* Mouse Cursor Position Relative to Window Callback */
    static void cursorPos_callback(GLFWwindow* window, double xPos, double yPos);

    /* Mouse Cursor Scroll Offset Callback */
    static void mouseScroll_callback(GLFWwindow* window, double xOffset, double yOffset);

    /* Window Resize Callback */
    static void windowResize_callback(GLFWwindow *window, int width, int height);

    /* GLFW Error Callback */
    static void error_callback(int error, const char* description);


  protected:  // Shared Overrideable Callbacks
    virtual void onKey(int key, int scancode, int action, int mods);
    virtual void onMouseClick(int button, int action, int mods);
    virtual void onMouseScroll(double xOffset, double yOffset);
    virtual void onMouse(double xPos, double yPos);
    virtual void onWindowResize(int width, int height);


  protected:  // Shared Methods
    /**
     * Returns the Calculated Frames Per Second from Draw Loop
     *	@returns FPS Value
    */
    const double getFPS();



  private:  // Helper Functions
    /**
     * Draw loop
     */
    virtual void Draw();

    /**
     * Data/Properties to configure prior to Drawing
     */
    virtual void Preload();

    /**
     * Sets up ImGui and Render it
     */
    virtual void drawImGui();

    /*
    * Fixed Interval Update
    * Used for Physics Sync
    *  @param deltaTime - Timestep
    */
    virtual void fixedUpdate(double deltaTime);



  public:  // Main User-End Operation Functions
    /**
     * Constuctor that initalizes GLFW with given Width and Height
     * @param w - The Width of the Window
     * @param h - The Height of the Window
     */
    SimpleRender(unsigned int w, unsigned int h, const char* title = "GLFW Window");

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
