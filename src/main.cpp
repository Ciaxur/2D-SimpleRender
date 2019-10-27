// Engine Libraries
#include "includes/SimpleRender.h"

// Core Libraries
#include <sys/stat.h>
#include <cmath>

// Helper Libraries
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

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


		  // TESTING: Texture Loading
		  {
			  GLfloat textureCoord[] = {
				  0.0f, 0.0f,	// Bottom Left
				  1.0f, 0.0f,	// Bottom Right
				  1.0f, 1.0f,	// Top    Right
				  0.0f, 1.0f,	// Top	  Left
			  };



			  // Load Image Texture with Correct Orientation
			  int width, height, channels;
			  stbi_set_flip_vertically_on_load(true);
			  unsigned char* data = stbi_load("Textures/615-checkerboard.png", &width, &height, &channels, 0);
			  std::cout << "Image Loaded: w[" << width << "] h[" << height << "]\n";

			  // Make sure data Loaded 
			  if (data) {
				  // Create Texture Buffer
				  GLuint texture;
				  glGenTextures(1, &texture);
				  glActiveTexture(GL_TEXTURE0);				// Set Texture to first Uniform Texture Sampler
				  glBindTexture(GL_TEXTURE_2D, texture);

				  // Setup Texture Options (Applies to Current Bound Object)
				  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
				  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


				  // Store the Data & Generate Mipmaps
				  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
				  glGenerateMipmap(GL_TEXTURE_2D);

				  // Store the Texture ID to lastest Buffer Data
				  bufferData.back().textureID = texture;
			  }

			  else {
				  std::cerr << "Texture: Image couldn't be loaded!\n";
			  }

			  stbi_image_free(data);


			  // Setup Attribute Data
			  GLuint aTexCoord = glGetAttribLocation(defaultShader.ID, "aTextCoord");
			  glVertexAttribPointer(aTexCoord, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
			  glEnableVertexAttribArray(aTexCoord);
		  }
	  }


	  /* Main Draw location of Application */
	  void Draw() {
		  if (defaultShader.status) {
			defaultShader.use();			// Use Default Program
		  }

		  for (BufferData& bd : bufferData) {
			  // Bind Texture
			  glActiveTexture(GL_TEXTURE0);
			  glBindTexture(GL_TEXTURE_2D, bd.textureID);

			  // Bind Vertex Array
			  glBindVertexArray(bd.VAO);		// Bind the Vertex Array
			  glEnableVertexAttribArray(0);		// Bind aPos Attribute to Vertex Array

			  // Draw
			  glDrawElements(GL_TRIANGLES, bd.indiciesElts, GL_UNSIGNED_INT, 0);
			  glBindVertexArray(0);

			  // Done with Active Attributes
			  glDisableVertexAttribArray(0);
		  }


		  // Live GLSL Shader Update
		  if (shaderUpdateActive) liveGLSLUpdateShaders();
	  }
};




int main() {
	//SimpleRender render(640, 480, "triangle opengl");
	//int status = render.run();
	//if (status != 0)
	//	std::cerr << "status = " << status << std::endl;

	App app(WIDTH, HEIGHT, "Triangle OpenGL");
	app.enableLiveShaderUpdate();

	int status = app.run();
	if (status != 0)
        std::cerr << "Status = " << status << std::endl;

    return 0;
}