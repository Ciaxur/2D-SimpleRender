// Engine Libraries
#include "SimpleRender.h"
#include "Shape.h"
#include "Rectangle.h"
#include "Circle.h"

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

    std::vector<Shape*> entities = {};


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
      :SimpleRender(width, height, title) {}

    ~App() {
      for (Shape *s : this->entities)
        if (s) delete s;
    }

    void enableLiveShaderUpdate() { shaderUpdateActive = true; }
    void disableLiveShaderUpdate() { shaderUpdateActive = false; }

    /* Configure/Load Data that will be used in Application */
    void Preload() {
      // Setting up entities.
      {
        // Custom shader.
        std::shared_ptr<Shader> shader = std::make_shared<Shader>();
        shader->compile("./shaders/shader.vert", "./shaders/shader2.frag");

        Shape *e = reinterpret_cast<Shape*>(new Rectangle{
          (WIDTH / 2.f) + 100.f, HEIGHT / 3.f,
          400.f, 350.f,
          shader,
          "./textures/615-checkerboard.png"
        });

        e->set_origin(e->get_center_vec());
        this->entities.push_back(e);
      }

      {
        std::shared_ptr<Shader> shader = std::make_shared<Shader>();
        shader->compile("./shaders/shader.vert", "./shaders/shader.frag");

        Shape *e = reinterpret_cast<Shape*>(new Rectangle{
          (WIDTH / 2.f) + - 450.f, HEIGHT / 3.f,
          400.f, 350.f,
          shader,
          "./textures/texture.png"
        });

        e->set_origin(e->get_center_vec());
        this->entities.push_back(e);
      }

      {
        std::shared_ptr<Shader> shader = std::make_shared<Shader>();
        shader->compile("./shaders/shader.vert", "./shaders/shader2.frag");

        Shape *e = reinterpret_cast<Shape*>(new Circle{
          (WIDTH / 2.f), (HEIGHT / 2.f) + 300.f,
          100.f,    // radius
          shader,
          "./textures/615-checkerboard.png",
          // nullptr,  // no texture
          2000       // quality = data points
        });

        // useSolidColor(shader.get(), glm::vec4(255.f, 0.f, 0.f, 255.f));
        e->set_origin(e->get_center_vec());
        this->entities.push_back(e);
      }

      spdlog::info("Loaded entities -> {}", this->entities.size());

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
      GLint utransfrom  = glGetUniformLocation(shader->ID, "transform");
      GLint u_time      = glGetUniformLocation(shader->ID, "u_time");
      GLint u_mouse     = glGetUniformLocation(shader->ID, "u_mouse");
      GLint u_res       = glGetUniformLocation(shader->ID, "u_res");

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

    void useSolidColor(Shader *shader, glm::vec4 vertexColor) {
      // Get uniform location for using a vertex color + flag to use it.
      GLint uniformUseTexture   = glGetUniformLocation(shader->ID, "useTexture");
      GLint uniformSolidColor   = glGetUniformLocation(shader->ID, "solidColor");

      // Toggle using the vertex color + set the color.
      glUniform1ui(uniformUseTexture, false);
      glUniform4f(uniformSolidColor, vertexColor.r, vertexColor.g, vertexColor.b, vertexColor.a);
    }

    /* Main Draw location of Application */
    void Draw() {
      // Output FPS to Window Title
      sprintf(titleBuffer, "%s [%.2f FPS]", title, getFPS());
      glfwSetWindowTitle(window, titleBuffer);


      // Translate them entities.
      double gl_time = glfwGetTime();
      glm::vec2 trans{sin(gl_time), 0.f};

      // Draw entities.
      for (Shape *entity : this->entities) {
        const BufferData &bd = entity->buffer;

        entity->translate(trans);
        entity->rotate(0.01f);
        entity->scale(glm::vec2{ 1.f + (float)(sin(gl_time) * 0.0015f) });
        entity->update();

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
        else useSolidColor(bd.shader.get(), glm::vec4(255.f, 0.f, 0.f, 255.f));

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
