// Engine Libraries
#include "includes/SimpleRender.h"

// Core Libraries
#include <sys/stat.h>

// Helper Libraries
#include <spdlog/spdlog.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// OpenGL Macros
#define WIDTH 800
#define HEIGHT 400


class App : public SimpleRender {
  private:
    time_t FshaderLastMod, VshaderLastMod;
    bool shaderUpdateActive = false;

    bool trackMouseMove = false;
    glm::vec2 prevMousePos = glm::vec2();
    float transX  = 0.0f;
    float transY  = 0.0f;
    float transZ  = 1.0f;
    float near    = 1.0f;


    /* Obtains last updated time of given file */
    static time_t getLastModified(const char* filename) {
      struct stat st;
      if (stat(filename, &st) == 0) {
        return st.st_mtime;
      }

      return -1;
    }

    /* Updates Shaders Live */
    void liveGLSLUpdateShaders() {
      // Check & Update Fragment Shader
      time_t newFshaderLastMod = getLastModified("./shaders/shader.frag");
      time_t newVshaderLastMod = getLastModified("./shaders/shader.vert");

      if (newFshaderLastMod != FshaderLastMod || newVshaderLastMod != VshaderLastMod) {
        FshaderLastMod = newFshaderLastMod;
        VshaderLastMod = newVshaderLastMod;

        // Re-Compile Shaders
        // Attach & Link Shaders & Use
        defaultShader.compile("./shaders/shader.vert", "./shaders/shader.frag");
      }
    }

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
      ImGui::TextColored(ImVec4(1.0f, 0.5f, 1.0f, 1.0f), "TransX: %.2f", transX);
      ImGui::TextColored(ImVec4(1.0f, 0.5f, 1.0f, 1.0f), "TransY: %.2f", transY);
      ImGui::TextColored(ImVec4(1.0f, 0.5f, 1.0f, 1.0f), "TransZ: %.2f", transZ);
      ImGui::TextColored(ImVec4(1.0f, 0.5f, 1.0f, 1.0f), "FPS: %.2f", this->getFPS());

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
      FshaderLastMod = VshaderLastMod = 0;
    }

    void enableLiveShaderUpdate() { shaderUpdateActive = true; }
    void disableLiveShaderUpdate() { shaderUpdateActive = false; }

    /* Configure/Load Data that will be used in Application */
    void Preload() {
      // Load in Default Shaders
      defaultShader.compile("./shaders/shader.vert", "./shaders/shader.frag");

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

        bufferData.push_back(
          CreateBuffer::static_float(verticies, sizeof(verticies), indicies, sizeof(indicies), defaultShader.ID));
      }

      // Setup Textures for Vert1
      bufferData.back().texture = new Texture("./textures/615-checkerboard.png");


      // Verticies 2
      {
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

        bufferData.push_back(
          CreateBuffer::static_float(verticies, sizeof(verticies), indicies, sizeof(indicies), defaultShader.ID));
      }


      // Setup Textures for Vert2
      bufferData.back().texture = new Texture("./textures/texture.png");


      // Display some Internal Info
      int nrAttribs;
      glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttribs);
      std::cout << "Maximum number of Vertex Attributes Supported: " << nrAttribs << std::endl;
    }

    void fixedUpdate(double dt) override {
      spdlog::info("Delta Time[{:.2f}]", dt);
    }

    /* Main Draw location of Application */
    void Draw() {
      if (defaultShader.status) {
        defaultShader.use();  // Use Default Program
      }

      // Output FPS to Window Title
      sprintf(titleBuffer, "%s [%.2f FPS]", title, getFPS());
      glfwSetWindowTitle(window, titleBuffer);

      // Transform Based on Input
      glm::mat4 trans(1.0f);
      trans = glm::ortho(-transZ, transZ, -transZ, transZ, -near, near);
      trans = glm::translate(trans, glm::vec3(transX, transY, 0.0f));

      GLuint utransfrom = glGetUniformLocation(defaultShader.ID, "transform");
      glUniformMatrix4fv(utransfrom, 1, GL_FALSE, glm::value_ptr(trans));



      // Draw From the Buffer
      for (BufferData& bd : bufferData) {
        // Enable aPos Attribute
        glEnableVertexAttribArray(0);

        // Bind Vertex Array Object
        glBindVertexArray(bd.VAO);

        // Bind Index Buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bd.indiciesBuffer);

        // Bind the Texture
        if (bd.texture)
          bd.texture->bind(0);


        // Draw
        glDrawElements(GL_TRIANGLES, bd.indiciesElts, GL_UNSIGNED_INT, nullptr);


        // Unbind the Texture
        if (bd.texture)
          bd.texture->unbind();

        // Disable aPos Attribute
        glDisableVertexAttribArray(0);
      }


      // Live GLSL Shader Update
      if (shaderUpdateActive) liveGLSLUpdateShaders();
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
