#include "SimpleRender.h"

/**
 ***********************************************************
 * Private Static Methods and Callbacks
 *  - Key Presses, Mouse Button, Cursor Movements, and
 *  Mouse Scrolling Callbacks
 ***********************************************************
 */

void SimpleRender::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
  // Obtain and Cast Object Reference
  void *r = glfwGetWindowUserPointer(window);
  if (r != NULL) {
    SimpleRender *obj = static_cast<SimpleRender *>(r);
    obj->onKey(key, scancode, action, mods);  // Call Overrideable Function
  }
}

void SimpleRender::onKey(int key, int scancode, int action, int mods) {
  // Output Key Pressed
  spdlog::info("KEY: Key[{}], ScanCode[{}], Action[{}], Mods[{}]\n", key, scancode, action, mods);

  // Close window on 'Q' Press
  if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
}

void SimpleRender::mouseBtn_callback(GLFWwindow *window, int button, int action, int mods) {
  // Obtain and Cast Object Reference
  void *r = glfwGetWindowUserPointer(window);
  if (r != NULL) {
    SimpleRender *obj = static_cast<SimpleRender *>(r);
    obj->onMouseClick(button, action, mods);  // Call Overrideable Function
  }
}

void SimpleRender::onMouseClick(int button, int action, int mods) {
  // Output Key Pressed
  spdlog::info("MOUSE: Button[{}], Action[{}], Mods[{}]", button, action, mods);
}

void SimpleRender::cursorPos_callback(GLFWwindow *window, double xPos, double yPos) {
  // Obtain and Cast Object Reference
  void *r = glfwGetWindowUserPointer(window);
  if (r != NULL) {
    SimpleRender *obj = static_cast<SimpleRender *>(r);

    // Update Mouse Data
    obj->mousePos.x = xPos;
    obj->mousePos.y = yPos;

    // Call Overrideable Function
    obj->onMouse(xPos, yPos);
  }
}

void SimpleRender::onMouse(double xPos, double yPos) {
  // Output Mouse Cursor Position
  spdlog::info("CURSOR: X[{:.1f}], Y[{:.1f}]", xPos, yPos);
}

void SimpleRender::mouseScroll_callback(GLFWwindow *window, double xOffset, double yOffset) {
  // Obtain and Cast Object Reference
  void *r = glfwGetWindowUserPointer(window);
  if (r != NULL) {
    SimpleRender *obj = static_cast<SimpleRender *>(r);
    obj->onMouseScroll(xOffset, yOffset);  // Call Overrideable Function
  }
}

void SimpleRender::onMouseScroll(double xOffset, double yOffset) {
  // Output Mouse Cursor Position
  spdlog::info("SCROLL: X-off[{:.2f}], Y-off[{:.2f}]", xOffset, yOffset);
}

void SimpleRender::windowResize_callback(GLFWwindow *window, int width, int height) {
  // Obtain and Cast Object Reference
  void *r = glfwGetWindowUserPointer(window);
  if (r != NULL) {
    SimpleRender *obj = static_cast<SimpleRender *>(r);
    obj->onWindowResize(width, height);  // Call Overrideable Function
  }
}

void SimpleRender::onWindowResize(int width, int height) {
  this->HEIGHT = height;
  this->WIDTH = width;
  glViewport(0, 0, width, height);
}


void SimpleRender::error_callback(int error, const char *description) {
  // Output any Errors
  spdlog::error("Error[{}]: {}", error, description);
}

const double SimpleRender::getFPS() {
  return FPS;
}


/**
 ***********************************************************
 * Private Helper Functions
 *
 *  - Draw Loop
 *  - Preload Function
 *  - Is Called prior to Draw
 *  - Process Input Function
 *  - Handles User Input from window
 *  - Function is Called Prior to Draw Loop
 *  - Fixed Update
 *  - Physics / Calculation Fixed Step Update
 ***********************************************************
 */

void SimpleRender::Draw() {
  // Draw the Object
  defaultShader.use();

  // Output FPS to Window Title
  sprintf(titleBuffer, "%s [%.2f FPS]", title, getFPS());
  glfwSetWindowTitle(window, titleBuffer);

  // Render all Buffer Data
  for (BufferData &bd : bufferData) {
    // Enable aPos Attribute
    glEnableVertexAttribArray(0);

    // Bind Vertex Array Object
    glBindVertexArray(bd.VAO);

    // Bind Indicies
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bd.indiciesBuffer);

    // Draw the Elements
    glDrawElements(GL_TRIANGLES, bd.indiciesElts, GL_UNSIGNED_INT, nullptr);

    // Finished with aPos Attribute
    glDisableVertexAttribArray(0);
  }
}

void SimpleRender::Preload() {
  // Load in Default Shaders
  defaultShader.compile("Shaders/shader.vert", "Shaders/shader.frag");

  // Create Object1
  GLfloat verticies[] = {
    // Positions<vec3>		RGBA<vec4>					// Texture Coordinates<vec2>
    -0.4f, -0.2f, 0.0f,   1.0f, 0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
    -0.2f, -0.2f, 0.0f,   0.0f, 1.0f, 0.0f, 1.0f,   1.0f, 0.0f,
    -0.4f, 0.2f, 0.0f,  0.0f, 0.0f, 1.0f, 1.0f,   0.0f, 1.0f,
    -0.2f, 0.2f, 0.0f,  1.0f, 1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
  };

  GLuint indicies[] = {
    0, 1, 2,  // First Triangle
    1, 2, 3   // Second Triangle
  };

  // Create and Bind Data to Buffer
  bufferData.push_back(
    CreateBuffer::static_float(verticies, sizeof(verticies), indicies, sizeof(indicies), defaultShader.ID)
  );


  // Create Object2
  GLfloat verticies2[] = {
    // Positions<vec3>		RGB<vec4>					// Texture Coordinates<vec2>
    0.0f, 0.3f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,   // Bottom-Left
    0.4f, 0.3f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,   // Bottom-Right
    0.0f, -0.3f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,  // Top-Left
    0.4f, -0.3f, 0.0f, 0.5f, 0.0f, 0.5f, 1.0f, 1.0f, 1.0f,  // Top-Right
  };
  bufferData.push_back(
    CreateBuffer::static_float(verticies2, sizeof(verticies2), indicies, sizeof(indicies), defaultShader.ID)
  );


  // DEBUG: Output Data Created
  int i = 0;
  for (BufferData &bd : bufferData) {
    std::cout << "Buffer[" << i << "]:\n";
    std::cout << "\tIndexBuffer: " << bd.indiciesBuffer << '\n';
    std::cout << "\tIndexElements: " << bd.indiciesElts << '\n';
    std::cout << "\tVertexBuffer: " << bd.verticiesBuffer << '\n';
    std::cout << "\tTextureID: " << bd.texture->textureID << "\n\n";
  }
}

void SimpleRender::fixedUpdate(double deltaTime) {}

void SimpleRender::drawImGui() {
  ImGui::ShowDemoWindow();

  // ImGui::Begin("Hello, world!");        // Create a window called "Hello, world!" and append into it.
  // ImGui::Text("This is some useful text.");     // Display some text (you can use a format strings too)
  // ImGui::SameLine();
  // ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
  // ImGui::End();
}


/**
 ***********************************************************
 * Main User-End Methods
 *
 *  - Constructor for Creating the Object :)
 *  - Destructor for Cleaning Up
 *  - Inizializing & Starting the Engine once the User
 *  is ready!
 ***********************************************************
 */

SimpleRender::SimpleRender(unsigned int w, unsigned int h, const char *title) : WIDTH(w), HEIGHT(h) {
  this->title = title;
  InitRender();
}

SimpleRender::~SimpleRender() {
  spdlog::info("Exiting, cleaning up first...");

  // Cleanup ImGui
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();


  /* Free Up Buffer Data */
  for (BufferData &bf : bufferData) {
    BufferData::freeBufferData(&bf);
  }

  /* Destroy Resources */
  glfwDestroyWindow(window);
  glfwTerminate();
}

void SimpleRender::InitRender() {
  /* Configure OpenGL Properties */
  glfwWindowHint(GLFW_SAMPLES, 4);        // 4x Antialiasing
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);      // OpenGL 3.3
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);      // OpenGL 3.3
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);    // Enables Forward Compatiblity
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // OpenGL Profile used
}

int SimpleRender::run() {
  /* Initialize GLFW */
  glewExperimental = true;  // Needed for Core Profile
  if (!glfwInit()) {
    spdlog::error("Failed to Initialize GLFW");
    glfwTerminate();
    return -1;
  }


  /* Open a Window with OpenGL Context */
  window = glfwCreateWindow(WIDTH, HEIGHT, title, NULL, NULL);
  glViewport(0, 0, WIDTH, HEIGHT);  // Set Rendering Dimensions

  if (!window) {
    spdlog::error("Failed to open GLFW window!");
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);  // Initialize GLEW


  /* Setup GLFW Properties */
  glfwSwapInterval(1);  // Default is 0, this is to prevent Tearing
  glEnable(GL_DEPTH_TEST);


  /* Initialize GLEW */
  if (glewInit() != GLEW_OK) {
    spdlog::error("Failed to Initalize GLEW");
    glfwTerminate();
    return -1;
  }

  /* Setup Input Callbacks */
  glfwSetWindowUserPointer(window, this);  // Keep track of Current Object
  glfwSetKeyCallback(window, key_callback);
  glfwSetMouseButtonCallback(window, mouseBtn_callback);
  glfwSetCursorPosCallback(window, cursorPos_callback);
  glfwSetScrollCallback(window, mouseScroll_callback);
  glfwSetWindowSizeCallback(window, windowResize_callback);
  glfwSetErrorCallback(error_callback);


  /* Setup ImGui */
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  ImGui::StyleColorsDark();

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 150");  // GLSL 3.2+


  /* Keep track of FPS & Fixed Upate */
  double lastTime = glfwGetTime();
  int frameCount = 0;

  /* Run Pre-Start Function */
  Preload();

  /* Get Uniform Locations */
  GLint u_time = glGetUniformLocation(this->defaultShader.ID, "u_time");
  GLint u_mouse = glGetUniformLocation(this->defaultShader.ID, "u_mouse");
  GLint u_res = glGetUniformLocation(this->defaultShader.ID, "u_res");

  /* Set Uniform Data */
  glm::vec2 v_res(this->WIDTH, this->HEIGHT);  // Set Resolution Vector


  /* Keep Window open until 'Q' key is pressed */
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
  do {
    // Measure the Speed (FPS)
    double currentTime = glfwGetTime();
    frameCount++;
    if (currentTime - lastTime >= 1.0) {
      fixedUpdate(currentTime - lastTime);
      FPS = frameCount;
      frameCount = 0;
      lastTime += 1.0;
    }

    // Start ImGui Frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();



    // Update Uniform Data
    glUniform1f(u_time, currentTime);
    glUniform2fv(u_res, 1, glm::value_ptr(v_res));  // A Single vec2 Float
    glUniform2fv(u_mouse, 1, glm::value_ptr(mousePos));


    // Clear the Screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Setup ImGui
    drawImGui();

    // Draw here...
    Draw();

    // Render ImGui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Swap Buffers
    glfwSwapBuffers(window);

    // Wait for Polling Events
    glfwPollEvents();
  } while (!glfwWindowShouldClose(window));  // Keep Window Open util Window should Closed

  // No Issues
  return 0;
}
