// Engine Libraries
#include "includes/SimpleRender.h"

// Core Libraries
#include <sys/stat.h>
#include <cmath>

// OpenGL Macros
#define WIDTH 800
#define HEIGHT 400


class App : public SimpleRender {
  private:
	  time_t FshaderLastMod, VshaderLastMod;
	  bool shaderUpdateActive = false;


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

  public:
	  App(unsigned int  width, unsigned int  height, const char* title)
		  : SimpleRender(width, height, title) {}

	  void enableLiveShaderUpdate() { shaderUpdateActive = true; }
	  void disableLiveShaderUpdate() { shaderUpdateActive = false; }



	  /* Configure/Load Data that will be used in Application */
	  void Preload() {
		  // Load in Default Shaders
		  defaultShader.compile("./Shaders/shader.vert", "./Shaders/shader.frag");


		  // Setting Up Verticies
		  {
			  GLfloat verticies[] = {
					-0.5f,  -0.5f, 0.0f,		// Bottom-Left
					-0.5f,   0.5f, 0.0f,		// Top   -Left
					 0.5f,   0.5f, 0.0f,		// Top	 -Right
					 0.5f,  -0.5f, 0.0f			// Bottom-Right
			  };

			  GLuint indicies[] = { 
				  3, 0, 1,
				  3, 2, 1
			  };

			  bufferData.push_back(
				  createBuffer(verticies, sizeof(verticies), indicies, sizeof(indicies))
			  );


			  // Display some Internal Info
			  int nrAttribs;
			  glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttribs);
			  std::cout << "Maximum number of Vertex Attributes Supported: " << nrAttribs << std::endl;
		  }
	  }


	  /* Main Draw location of Application */
	  void Draw() {
		  if (defaultShader.status) {
			defaultShader.use();			// Use Default Program
		  }


		  

		  for (BufferData& bd : bufferData) {
			  // Enable aPos Attribute
			  glEnableVertexAttribArray(0);

			  // Bind Vertex Array Object
			  glBindVertexArray(bd.VAO);

			  // Bind Index Buffer
			  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bd.indiciesBuffer);

			  // Draw
			  glDrawElements(GL_TRIANGLES, bd.indiciesElts, GL_UNSIGNED_INT, nullptr);

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