// Engine Libraries
#include "includes/SimpleRender.h"

// Helper Libraries
#include <spdlog/spdlog.h>

// Graphics libraries.
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// OpenGL Macros
#define WIDTH 1600
#define HEIGHT 900


class App : public SimpleRender {
  private:
    bool shaderUpdateActive = false;
    bool trackMouseMove = false;
    glm::vec2 prevMousePos = glm::vec2();
    float transX  = 0.0f;
    float transY  = 0.0f;
    float transZ  = 1.0f;
    float near    = 1.0f;

    void onKey(int key, int scancode, int action, int mods) {
      float offset = 0.01f;

      // Adjust Transformation
      if (action == GLFW_REPEAT || action == GLFW_PRESS) {
        if (key == GLFW_KEY_LEFT)
          transX -= offset;
        else if (key == GLFW_KEY_RIGHT)
          transX += offset;

        else if (key == GLFW_KEY_UP)
          transY += offset;
        else if (key == GLFW_KEY_DOWN)
          transY -= offset;

        else if (key == GLFW_KEY_Q)
          glfwSetWindowShouldClose(window, GLFW_TRUE);
      }


      // Log Data
      spdlog::info("TransX[{:.2f}] \t TransY[{:.2f}]", transX, transY);
    }

    void onMouseClick(int button, int action, int mods) {
      // ImGui Captured Mouse
      if (ImGui::GetIO().WantCaptureMouse) return;

      if(button == 0) { // Left-Click
        trackMouseMove = action;
      }
    }

    void onMouse(double xPos, double yPos) {
      // Track mouse movement IF not captured by ImGUI
      if (trackMouseMove) {
        transX -= ((prevMousePos.x - xPos) / WIDTH )  * transZ;     // Transform with respect to Z
        transY += ((prevMousePos.y - yPos) / HEIGHT)  * transZ;
      }

      // Keep Track of Previous xy Position
      prevMousePos.x = xPos;
      prevMousePos.y = yPos;
    }

    void onMouseScroll(double xOffset, double yOffset) {
      // Zoom in/out by Transforming Z-Axis
      if (yOffset > 0.0f)
        transZ -= 0.05f;
      else if (yOffset < 0.0f)
        transZ += 0.05f;
    }

    void drawImGui() {
      ImGui::Begin("Debug Menu");
      ImGui::TextColored(ImVec4(1.0f, 0.5f, 1.0f, 1.0f), "Mouse: [x=%.2f|y=%.2f]", prevMousePos.x, prevMousePos.y);
      ImGui::TextColored(ImVec4(1.0f, 0.5f, 1.0f, 1.0f), "TransX: %.2f", transX);
      ImGui::TextColored(ImVec4(1.0f, 0.5f, 1.0f, 1.0f), "TransY: %.2f", transY);
      ImGui::TextColored(ImVec4(1.0f, 0.5f, 1.0f, 1.0f), "TransZ: %.2f", transZ);
      ImGui::TextColored(ImVec4(1.0f, 0.5f, 1.0f, 1.0f), "FPS: %.2f", this->getFPS());

      // Window dimensions.
      int width, height;
      glfwGetWindowSize(this->getWindow(), &width, &height);
      ImGui::TextColored(ImVec4(1.0f, 0.5f, 1.0f, 1.0f), "Window: %dx%d", width, height);

      ImGui::BeginGroup();
      ImGui::TextColored(ImVec4(1.0f, 0.5f, 1.0f, 1.0f), "Near: %.2f", near);
      ImGui::SameLine();
      if (ImGui::SmallButton("-"))
        near -= 0.01f;
      ImGui::SameLine();

      if (ImGui::SmallButton("+"))
        near += 0.01f;
      ImGui::EndGroup();

      ImGui::End();
    }

  public:
    App(unsigned int width, unsigned int height, const char* title)
      : SimpleRender(width, height, title) {
        this->bufferData.clear();
      }

    void enableLiveShaderUpdate() { shaderUpdateActive = true; }
    void disableLiveShaderUpdate() { shaderUpdateActive = false; }

    /* Configure/Load Data that will be used in Application */
    void Preload() {
      // Setting Up Verticies
      {
        GLfloat verticies[] = {
          // VERTEX<vec3>		  RGBA<vec4>					      // Texture Coordinates<vec2>
          0.2f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
          0.2f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f,   0.0f, 1.0f,
          0.8f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f, 1.0f,   1.0f, 1.0f,
          0.8f, -0.5f, 0.0f,  0.0f, 0.0f, 0.0f, 1.0f,   1.0f, 0.0f
        };

        GLuint indicies[] = {
          3, 0, 1,
          3, 2, 1
        };

        // Custom shader.
        std::shared_ptr<Shader> shader = std::make_shared<Shader>();
        shader->compile("./shaders/shader.vert", "./shaders/shader2.frag");

        BufferData buffer = CreateBuffer::dynamic_float(verticies, sizeof(verticies), indicies, sizeof(indicies), shader);
        buffer.texture = new Texture("./textures/615-checkerboard.png");
        bufferData.push_back(buffer);;
      }

      // Verticies 2
      {
        std::shared_ptr<Shader> shader = std::make_shared<Shader>();
        shader->compile("./shaders/shader.vert", "./shaders/shader.frag");

        GLfloat verticies[] = {
          // VERTEX<vec3>		    RGBA<vec4>					      // Texture Coordinates<vec2>
          -0.8f, -0.5f,  0.0f,  1.0f, 0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
          -0.8f,  0.5f,  0.0f,  0.0f, 1.0f, 0.0f, 1.0f,   0.0f, 1.0f,
          -0.2f,  0.5f,  0.0f,  0.0f, 0.0f, 1.0f, 1.0f,   1.0f, 1.0f,
          -0.2f, -0.5f,  0.0f,  0.0f, 0.0f, 0.0f, 1.0f,   1.0f, 0.0f
        };

        GLuint indicies[] = {
          3, 0, 1,
          3, 2, 1
        };

        BufferData buffer = CreateBuffer::dynamic_float(verticies, sizeof(verticies), indicies, sizeof(indicies), shader);
        buffer.texture = new Texture("./textures/texture.png");
        bufferData.push_back(buffer);
      }

      spdlog::info("Loaded buffers -> {}", bufferData.size());

      // Display some Internal Info
      int nrAttribs;
      glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttribs);
      spdlog::info("Maximum number of Vertex Attributes Supported: {}", nrAttribs);
    }

    void fixedUpdate(double dt) override {
      spdlog::info("Delta Time[{:.2f}]", dt);
    }

    void updateUniforms(Shader *shader) {
      // Transform Based on Input
      glm::mat4 trans(1.0f);
      trans = glm::ortho(-transZ, transZ, -transZ, transZ, -near, near);
      trans = glm::translate(trans, glm::vec3(transX, transY, 0.0f));

      /* Get Uniform Locations */
      GLuint utransfrom = glGetUniformLocation(shader->ID, "transform");
      GLint u_time = glGetUniformLocation(shader->ID, "u_time");
      GLint u_mouse = glGetUniformLocation(shader->ID, "u_mouse");
      GLint u_res = glGetUniformLocation(shader->ID, "u_res");

      // Pass in the canvas transform.
      glUniformMatrix4fv(utransfrom, 1, GL_FALSE, glm::value_ptr(trans));

      // Update Uniform Data
      glUniform1f(u_time, glfwGetTime());

      // Set Resolution Vector
      int width, height;
      glfwGetWindowSize(this->getWindow(), &width, &height);
      glm::vec2 v_res(width, height);
      glUniform2fv(u_res, 1, glm::value_ptr(v_res));  // A Single vec2 Float

      // Mouse position.
      glUniform2fv(u_mouse, 1, glm::value_ptr(this->getMousePos()));
    }

    /* Main Draw location of Application */
    void Draw() {
      // Output FPS to Window Title
      sprintf(titleBuffer, "%s [%.2f FPS]", title, getFPS());
      glfwSetWindowTitle(window, titleBuffer);


      // Draw From the Buffer
      for (BufferData& bd : bufferData) {
        // Activate the bound shader program.
        bd.shader->use();

        // Pass in the uniform values into each of the vertex shader programs.
        updateUniforms(bd.shader.get());

        // Enable aPos Attribute
        glEnableVertexAttribArray(0);

        // Bind Vertex Array Object
        glBindVertexArray(bd.VAO);

        // Bind Index Buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bd.indiciesBuffer);

        // Bind the Texture
        if (bd.texture) bd.texture->bind(0);

        // Draw
        glDrawElements(GL_TRIANGLES, bd.indiciesElts, GL_UNSIGNED_INT, nullptr);

        // Unbind the Texture
        if (bd.texture) bd.texture->unbind();

        // Disable aPos Attribute
        glDisableVertexAttribArray(0);

        // Deactivate shader program.
        glUseProgram(0);

        // Live update each shader on mod.
        if (this->shaderUpdateActive) bd.shader->liveGLSLUpdateShaders();
      }
    }
};


int main() {
  spdlog::info("Welcome to spdlog version {}.{}.{}!", SPDLOG_VER_MAJOR, SPDLOG_VER_MINOR, SPDLOG_VER_PATCH);

  App app(WIDTH, HEIGHT, "2D Simple Render");
  app.enableLiveShaderUpdate();

  int status = app.run();
  if (status != 0)
    std::cerr << "Status = " << status << std::endl;

  return 0;
}
