// Engine Libraries
#include "Circle.h"
#include "Polygon.h"
#include "Rectangle.h"
#include "ShapeFromObjFile.h"
#include "SimpleRender.h"
#include "Shape.h"
#include "glm/ext/vector_float2.hpp"
#include "glm/gtc/constants.hpp"
#include "imgui.h"
#include "spdlog/fmt/bundled/format.h"

// Helper Libraries
#include <spdlog/spdlog.h>

// Graphics libraries.
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>

// OpenGL Macros
#define WIDTH 1600
#define HEIGHT 900


class App : public SimpleRender {
  private:
    bool shaderUpdateActive = false;
    bool trackMouseMove = false;
    glm::vec2 prevMousePos = glm::vec2();

    // Zoom percent offset.
    const double transZ_percent_offset = 0.05; // 5%

    double transX  = 0.0f;
    double transY  = 0.0f;
    double transZ  = 1.25f;
    double near    = 1.0f;

    std::vector<std::shared_ptr<Shape>> entities = {};
    std::vector<std::shared_ptr<Shape>> debug_entities = {};

    void onKey(int key, int scancode, int action, int mods) override {
      double offset = 0.01f;

      // Adjust Transformation
      if (action == GLFW_REPEAT || action == GLFW_PRESS) {
        if (key == GLFW_KEY_LEFT)
          transX += offset;
        else if (key == GLFW_KEY_RIGHT)
          transX -= offset;

        else if (key == GLFW_KEY_UP)
          transY -= offset;
        else if (key == GLFW_KEY_DOWN)
          transY += offset;

        else if (key == GLFW_KEY_Q)
          glfwSetWindowShouldClose(window, GLFW_TRUE);
      }


      // Log Data
      spdlog::info("TransX[{:.2f}] \t TransY[{:.2f}]", transX, transY);
    }

    void onMouseClick(int button, int action, int mods) override {
      // ImGui Captured Mouse
      if (ImGui::GetIO().WantCaptureMouse) return;

      if(button == 0) { // Left-Click
        trackMouseMove = action;
      }
    }

    void onMouse(double xPos, double yPos) override {
      // Track mouse movement IF not captured by ImGUI
      if (trackMouseMove) {
        transX -= ((prevMousePos.x - xPos) / WIDTH )  * transZ;     // Transform with respect to Z
        transY += ((prevMousePos.y - yPos) / HEIGHT)  * transZ;
      }

      // Keep Track of Previous xy Position
      prevMousePos.x = xPos;
      prevMousePos.y = yPos;
    }

    void onMouseScroll(double xOffset, double yOffset) override {
      // Skip if swallowed by imgui.
      if ( ImGui::GetIO().WantCaptureMouse ) { return; }

      // Zoom in/out by Transforming Z-Axis
      const double zoom_offset = (transZ * transZ_percent_offset);

      if (yOffset > 0.0f)
        transZ -= zoom_offset;
      else if (yOffset < 0.0f)
        transZ += zoom_offset;
    }

    void draw_buffer_submenu( const std::shared_ptr<BufferData> buffer ) {
      if ( ImGui::TreeNode( "buffer info" ) ) {
        if ( ImGui::BeginTable( "buffer_info_table", 2 ) ) {
          ImGui::TableNextRow();
          ImGui::TableNextColumn();
          ImGui::Text( "vao.id" );
          ImGui::TableNextColumn();
          ImGui::Text( "%u", buffer->vao_index );

          ImGui::TableNextRow();
          ImGui::TableNextColumn();
          ImGui::Text( "vertex.id" );
          ImGui::TableNextColumn();
          ImGui::Text( "%u", buffer->vertex_buffer_ptr->get_gl_buffer_index() );

          ImGui::TableNextRow();
          ImGui::TableNextColumn();
          ImGui::Text( "vertex.stride" );
          ImGui::TableNextColumn();
          ImGui::Text( "%u", buffer->vertex_stride );

          ImGui::TableNextRow();
          ImGui::TableNextColumn();
          ImGui::Text( "vertex.size" );
          ImGui::TableNextColumn();
          ImGui::Text( "%zu", buffer->vertex_buffer_ptr->size() );

          ImGui::TableNextRow();
          ImGui::TableNextColumn();
          ImGui::Text( "vertex.points" );
          ImGui::TableNextColumn();
          ImGui::Text( "%zu", buffer->vertex_buffer_ptr->size() / buffer->vertex_stride );

          ImGui::TableNextRow();
          ImGui::TableNextColumn();
          ImGui::Text( "vertex.bytes" );
          ImGui::TableNextColumn();
          ImGui::Text( "%zu", buffer->vertex_buffer_ptr->size_in_bytes() );

          ImGui::TableNextRow();
          ImGui::TableNextColumn();
          ImGui::Text( "index.size" );
          ImGui::TableNextColumn();
          ImGui::Text( "%zu", buffer->index_buffer_ptr->buffers.size() );

          ImGui::EndTable();
        }
        ImGui::TreePop();
      }

      if ( ImGui::TreeNode( "vertex.data" ) ) {
        if ( ImGui::BeginTable( "buffer_vertex_data_table", 3 ) ) {
          ImGui::TableNextRow();
          ImGui::TableNextColumn();
          ImGui::Text( "index" );
          ImGui::TableNextColumn();
          ImGui::Text( "x" );
          ImGui::TableNextColumn();
          ImGui::Text( "y" );

          for ( size_t i = 0; i < buffer->vertex_buffer_ptr->size(); i += buffer->vertex_stride ) {
            const double x = buffer->vertex_buffer_ptr->get_buffer()[i];
            const double y = buffer->vertex_buffer_ptr->get_buffer()[i + 1];

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Selectable(
              fmt::format( "{}", static_cast<size_t>( i / buffer->vertex_stride ) ).c_str(), false,
              ImGuiSelectableFlags_SpanAllColumns
            );
            if ( ImGui::IsItemHovered() && this->debug_entities.size() ) {
              for ( const auto &e : this->debug_entities ) { e->set_hidden( true ); }
              this->debug_entities[0]->set_position({ x, y, 0.0 });
              this->debug_entities[0]->set_hidden( false );
            }

            ImGui::TableNextColumn();
            ImGui::Text( "%.2f", x );
            ImGui::TableNextColumn();
            ImGui::Text( "%.2f", y );
          }

          ImGui::EndTable();
        }
        ImGui::TreePop();
      }

      if ( ImGui::TreeNode( "index.data" ) ) {
        for ( const auto &ibuf : buffer->index_buffer_ptr->buffers ) {
          if ( ImGui::TreeNode( fmt::format( "buffer.id({})", ibuf->get_gl_buffer_index() ).c_str() ) ) {
            if ( ImGui::BeginTable( fmt::format( "index_buffer_data_{}", ibuf->get_gl_buffer_index() ).c_str(), 3 ) ) {
              for ( size_t i = 0; i < ibuf->size(); i += 3 ) {
                const glm::vec<3, GLuint> ivec{
                  ibuf->get_buffer()[i],
                  ibuf->get_buffer()[i + 1],
                  ibuf->get_buffer()[i + 2]
                };

                ImGui::TableNextRow();
                ImGui::TableNextColumn();

                // Make rows highlight-able.
                ImGui::Selectable(
                  fmt::format( "{}", ivec.x ).c_str(), false, ImGuiSelectableFlags_SpanAllColumns
                );

                if ( ImGui::IsItemHovered() && this->debug_entities.size() >= 3 ) {
                  const glm::vec2 p0{
                    buffer->vertex_buffer_ptr->get_buffer()[( ivec.x * buffer->vertex_stride ) + 0],
                    buffer->vertex_buffer_ptr->get_buffer()[( ivec.x * buffer->vertex_stride ) + 1],
                  };
                  const glm::vec2 p1{
                    buffer->vertex_buffer_ptr->get_buffer()[( ivec.y * buffer->vertex_stride ) + 0],
                    buffer->vertex_buffer_ptr->get_buffer()[( ivec.y * buffer->vertex_stride ) + 1],
                  };
                  const glm::vec2 p2{
                    buffer->vertex_buffer_ptr->get_buffer()[( ivec.z * buffer->vertex_stride ) + 0],
                    buffer->vertex_buffer_ptr->get_buffer()[( ivec.z * buffer->vertex_stride ) + 1],
                  };

                  this->debug_entities[0]->set_position( glm::vec3{ p0, 0.0 } );
                  this->debug_entities[1]->set_position( glm::vec3{ p1, 0.0 } );
                  this->debug_entities[2]->set_position( glm::vec3{ p2, 0.0 } );

                  for ( const auto &e : this->debug_entities ) { e->set_hidden( true ); }
                  this->debug_entities[0]->set_hidden( false );
                  this->debug_entities[1]->set_hidden( false );
                  this->debug_entities[2]->set_hidden( false );
                }
                ImGui::TableNextColumn();
                ImGui::Text( "%u", ivec.y );
                ImGui::TableNextColumn();
                ImGui::Text( "%u", ivec.z );
              }
              ImGui::EndTable();
            }

            ImGui::TreePop();
          }
        }

        ImGui::TreePop();
      }
    }

    void draw_asset_menu() {
      ImGui::Begin( "Asset Menu" );

      for ( size_t i = 0 ; i < this->entities.size(); i++ ) {
        const auto& entity = this->entities[i];

        if ( ImGui::TreeNode( fmt::format( "Entity {}", i ).c_str() ) ) {
          if ( ImGui::TreeNode( "origin" ) ) {
            if ( ImGui::BeginTable(fmt::format( "entity_table_{}", i ).c_str(), 2) ) {
              ImGui::TableNextRow();
              ImGui::TableNextColumn();
              ImGui::Text("x");
              ImGui::TableNextColumn();
              ImGui::Text("%.2f", entity->get_origin().x);

              ImGui::TableNextRow();
              ImGui::TableNextColumn();
              ImGui::Text("y");
              ImGui::TableNextColumn();
              ImGui::Text("%.2f", entity->get_origin().y);

              ImGui::EndTable();
            }

            ImGui::TreePop();
          }

          draw_buffer_submenu(entity->buffer);
          ImGui::TreePop();
        }
      }

      ImGui::End();
    }

    void draw_debug_menu() {
      constexpr ImVec4 TEXT_PURPLE_COLOR = ImVec4(1.0f, 0.5f, 1.0f, 1.0f);

      ImGui::Begin("Debug Menu");
      {
        ImGui::TextColored(TEXT_PURPLE_COLOR, "Mouse: [x=%.2f|y=%.2f]", prevMousePos.x, prevMousePos.y);
        ImGui::TextColored(TEXT_PURPLE_COLOR, "TransX: %.2f", transX);
        ImGui::TextColored(TEXT_PURPLE_COLOR, "TransY: %.2f", transY);
        ImGui::TextColored(TEXT_PURPLE_COLOR, "TransZ: %.2f", transZ);
        ImGui::TextColored(TEXT_PURPLE_COLOR, "FPS: %.2f", this->getFPS());
        ImGui::TextColored(TEXT_PURPLE_COLOR, "Debug Entities: %zu", this->debug_entities.size());
      }

      // Window dimensions.
      {
        int width, height;
        glfwGetWindowSize(this->getWindow(), &width, &height);
        ImGui::TextColored(TEXT_PURPLE_COLOR, "Window: %dx%d", width, height);
      }

      // Transformation
      {
        ImGui::BeginGroup();
        ImGui::TextColored(TEXT_PURPLE_COLOR, "Near: %.2f", near);
        ImGui::SameLine();
        if (ImGui::SmallButton("-"))
          near -= 0.01f;
        ImGui::SameLine();

        if (ImGui::SmallButton("+"))
          near += 0.01f;
        ImGui::EndGroup();
      }

      // Rasterization modes.
      {
        ImGui::TextColored(TEXT_PURPLE_COLOR, "Rasterization: ");

        ImGui::SameLine();
        if (ImGui::SmallButton("Point")) {
          glPolygonMode(GL_FRONT, GL_POINT);
          glPolygonMode(GL_BACK, GL_POINT);
        }
        ImGui::SameLine();
        if (ImGui::SmallButton("Line")) {
          glPolygonMode(GL_FRONT, GL_LINE);
          glPolygonMode(GL_BACK, GL_LINE);
        }
        ImGui::SameLine();
        if (ImGui::SmallButton("Fill")) {
          glPolygonMode(GL_FRONT, GL_FILL);
          glPolygonMode(GL_BACK, GL_FILL);
        }
      }

      ImGui::End();
    }

    void drawImGui() override {
      draw_debug_menu();
      draw_asset_menu();
    }

  public:
    App(unsigned int width, unsigned int height, const char* title)
      :SimpleRender(width, height, title) {}

    ~App() {}

    void enableLiveShaderUpdate() { shaderUpdateActive = true; }
    void disableLiveShaderUpdate() { shaderUpdateActive = false; }

    /* Configure/Load Data that will be used in Application */
    void Preload() override {
      // Create 3 debug circles.
      {
        const glm::vec4 colors[3] = {
          glm::vec4( 255.f, 0.f, 0.f, 255.f ),
          glm::vec4( 0.f, 255.f, 0.f, 255.f ),
          glm::vec4( 0.f, 0.f, 255.f, 255.f ),
        };

        for ( size_t i = 0; i < 3; i++ ) {
          std::shared_ptr<Shader> shader = std::make_shared<Shader>();
          shader->compile( "./shaders/shader.vert", "./shaders/shader2.frag" );

          auto circle = Circle::create(
            ( WIDTH / 2.f ), ( HEIGHT / 2.f ) + 300.f,
            10.f,  // radius
            shader, nullptr,
            2000   // quality = data points
          );
          std::shared_ptr<Shape> e = std::move( circle );

          e->buffer->solid_color = colors[i];

          e->set_origin( e->get_center_vec() );
          this->debug_entities.push_back( e );

          // set to hidden by default.
          e->set_hidden( true );
        }
      }

      // Setting up entities.
      {
        // Custom shader.
        std::shared_ptr<Shader> shader = std::make_shared<Shader>();
        shader->compile("./shaders/shader.vert", "./shaders/shader2.frag");

        auto rect = (Rectangle::create(
          (WIDTH / 2.f) + 100.f, HEIGHT / 3.f,
          400.f, 350.f,
          shader,
          "./textures/615-checkerboard.png"
        ));

        std::shared_ptr<Shape> e = std::move(rect);

        e->set_origin(e->get_center_vec());
        this->entities.push_back(e);
      }

      {
        std::shared_ptr<Shader> shader = std::make_shared<Shader>();
        shader->compile("./shaders/shader.vert", "./shaders/shader.frag");

        auto rect = (Rectangle::create(
          (WIDTH / 2.f) + - 450.f, HEIGHT / 3.f,
          400.f, 350.f,
          shader,
          "./textures/texture.png"
        ));
        std::shared_ptr<Shape> e = std::move( rect );

        e->set_origin(e->get_center_vec());
        this->entities.push_back(e);
      }

      {
        std::shared_ptr<Shader> shader = std::make_shared<Shader>();
        shader->compile("./shaders/shader.vert", "./shaders/shader2.frag");

        double x = (WIDTH / 2.f);
        double y = (HEIGHT / 2.f) - 200.f;

        auto poly = Polygon::create(
          {
            {x,           y},
            {x + 100.0,   y},
            {x + 100.0,   y - 200.0},
            {x,           y - 100.0},
            {x + 100.0,   y - 100.0},
            {x + 200.0,   y - 100.0}
          },
          shader,
          "./textures/615-checkerboard.png"
        );

        std::shared_ptr<Shape> e = std::move( poly );
        this->entities.push_back(e);
      }

      {
        std::shared_ptr<Shader> shader = std::make_shared<Shader>();
        shader->compile( "./shaders/shader.vert", "./shaders/shader2.frag" );

        auto circle = Circle::create(
          ( WIDTH / 2.f ), ( HEIGHT / 2.f ) + 300.f,
          100.f,  // radius
          shader,
          "./textures/615-checkerboard.png",
          2000  // quality = data points
        );
        std::shared_ptr<Shape> e = std::move( circle );

        // set fallback color
        e->buffer->solid_color = glm::vec4(255.f, 0.f, 0.f, 255.f);
        e->set_origin( e->get_center_vec() );
        this->entities.push_back( e );
      }

      // WIP: Needs work
      {
        auto obj                 = ShapeFromObjFile::create( "./assets/tinker.obj" );
        std::shared_ptr<Shape> e = std::move( obj );
        this->entities.push_back( e );
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

    void _draw_shape(const Shape* shape) {
        // Skip hidden shapes.
        if ( shape->is_hidden() ) { return; }

        auto &bd = shape->buffer;

        // Translate them entities.
        // double gl_time = glfwGetTime();
        // glm::vec2 trans{sin(gl_time), 0.f};
        // entity->translate(trans);
        // entity->rotate(0.01f);
        // entity->scale(glm::vec2{ 1.f + (float)sin(gl_time) * 0.0015f });
        // entity->update();

        // Activate the bound shader program.
        bd->shader->use();

        // Pass in the uniform values into each of the vertex shader programs.
        updateUniforms(bd->shader.get());

        // Enable aPos Attribute
        glEnableVertexAttribArray(0);

        // Bind Vertex Array Object
        glBindVertexArray(bd->vao_index);

        // Bind the Texture
        if (bd->texture) { bd->texture->bind(0);
        } else {
          useSolidColor( bd->shader.get(), bd->solid_color );
        }

        // PERFORMANCE: refactor index buffer struct to use multiple buffers.
        // see: https://docs.gl/gl3/glGenBuffers
        for ( const auto& ibuf : bd->index_buffer_ptr->buffers ) {
          // Bind Index Buffer
          glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibuf->get_gl_buffer_index());

          // Draw
          for ( auto &buffer : bd->index_buffer_ptr->buffers ) {
            glDrawElements( GL_TRIANGLES, buffer->size(), GL_UNSIGNED_INT, nullptr );
          }
        }

        // Unbind the Texture
        if (bd->texture) bd->texture->unbind();

        // Disable aPos Attribute
        glDisableVertexAttribArray(0);

        // Deactivate shader program.
        glUseProgram(0);

        // Live update each shader on mod.
        if (this->shaderUpdateActive) bd->shader->liveGLSLUpdateShaders();
    }

    /* Main Draw location of Application */
    void Draw() override {
      // Output FPS to Window Title
      sprintf(titleBuffer, "%s [%.2f FPS]", title, getFPS());
      glfwSetWindowTitle(window, titleBuffer);

      // Draw entities.
      for ( auto &entity : this->debug_entities ) { this->_draw_shape( entity.get() ); }
      for ( auto &entity : this->entities ) { this->_draw_shape( entity.get() ); }
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
