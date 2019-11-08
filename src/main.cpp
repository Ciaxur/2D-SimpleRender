// Engine Libraries
#include "includes/SimpleRender.h"

// Core Libraries
#include <sys/stat.h>

// Helper Libraries
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
	  float transX = 0.0f;
	  float transY = 0.0f;


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
		  time_t newFshaderLastMod = getLastModified("./Shaders/shader.frag");
		  time_t newVshaderLastMod = getLastModified("./Shaders/shader.vert");

		  if (newFshaderLastMod != FshaderLastMod || newVshaderLastMod != VshaderLastMod) {
			  FshaderLastMod = newFshaderLastMod;
			  VshaderLastMod = newVshaderLastMod;

			  // Re-Compile Shaders
			  // Attach & Link Shaders & Use
			  defaultShader.compile("./Shaders/shader.vert", "./Shaders/shader.frag");
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
		  }


		  // Log Data
		  printf("TransX[%.2f] \t TransY[%.2f]\n", transX, transY);
	  }


  public:
	  App(unsigned int  width, unsigned int  height, const char* title)
		  : SimpleRender(width, height, title) {
		  FshaderLastMod = VshaderLastMod = 0;
	  }


	  void enableLiveShaderUpdate() { shaderUpdateActive = true; }
	  void disableLiveShaderUpdate() { shaderUpdateActive = false; }


	  /* Configure/Load Data that will be used in Application */
	  void Preload() {
		  // Load in Default Shaders
		  defaultShader.compile("./Shaders/shader.vert", "./Shaders/shader.frag");

		  // Setting Up Verticies
		  {
			  GLfloat verticies[] = {
				  // VERTEX<vec3>		RGBA<vec4>					// Texture Coordinates<vec2>
				   0.2f, -0.5f, 0.0f,	1.0f, 0.0f, 0.0f, 1.0f,		0.0f, 0.0f,
				   0.2f,  0.5f, 0.0f,	0.0f, 1.0f, 0.0f, 1.0f,		0.0f, 1.0f,
				   0.8f,  0.5f, 0.0f,	0.0f, 0.0f, 1.0f, 1.0f,		1.0f, 1.0f,
				   0.8f, -0.5f, 0.0f,	0.0f, 0.0f, 0.0f, 1.0f,		1.0f, 0.0f
			  };

			  GLuint indicies[] = { 
				  3, 0, 1,
				  3, 2, 1
			  };

			  bufferData.push_back(
				  createBuffer(verticies, sizeof(verticies), indicies, sizeof(indicies), defaultShader.ID)
			  );
		  }

		  // Setup Textures for Vert1
		  bufferData.back().texture = new Texture("./Textures/615-checkerboard.png");


		  // Verticies 2
		  {
			  GLfloat verticies[] = {
				  // VERTEX<vec3>		RGBA<vec4>					// Texture Coordinates<vec2>
				  -0.8f, -0.5f, -1.0f,	1.0f, 0.0f, 0.0f, 1.0f,		0.0f, 0.0f,
				  -0.8f,  0.5f, -1.0f,	0.0f, 1.0f, 0.0f, 1.0f,		0.0f, 1.0f,
				  -0.2f,  0.5f, -1.0f,	0.0f, 0.0f, 1.0f, 1.0f,		1.0f, 1.0f,
				  -0.2f, -0.5f, -1.0f,	0.0f, 0.0f, 0.0f, 1.0f,		1.0f, 0.0f
			  };

			  GLuint indicies[] = {
				  3, 0, 1,
				  3, 2, 1
			  };

			  bufferData.push_back(
				  createBuffer(verticies, sizeof(verticies), indicies, sizeof(indicies), defaultShader.ID)
			  );
		  }


		  // Setup Textures for Vert2
		  bufferData.back().texture = new Texture("./Textures/texture.png");
		  

		  // Display some Internal Info
		  int nrAttribs;
		  glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttribs);
		  std::cout << "Maximum number of Vertex Attributes Supported: " << nrAttribs << std::endl;
	  }


	  /* Main Draw location of Application */
	  void Draw() {
		  if (defaultShader.status) {
			defaultShader.use();			// Use Default Program
		  }

		  // Output FPS to Window Title
		  sprintf(titleBuffer, "%s [%.2f FPS]", title, getFPS());
		  glfwSetWindowTitle(window, titleBuffer);

		  // Transform Based on Input
		  glm::mat4 trans(1.0f);
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
			  if(bd.texture)
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
	App app(WIDTH, HEIGHT, "Triangle OpenGL");
	app.enableLiveShaderUpdate();

	int status = app.run();
	if (status != 0)
        std::cerr << "Status = " << status << std::endl;

    return 0;
}