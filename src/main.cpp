// Engine Libraries
#include "includes/SimpleRender.h"
#include "includes/Vector.h"

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
			  GLuint vertShader = InitShader("./Shaders/shader.vert", GL_VERTEX_SHADER);
			  GLuint fragShader = InitShader("./Shaders/shader.frag", GL_FRAGMENT_SHADER);
			  GLuint newProgramID;

			  // Attach & Link Shaders
			  if (vertShader != -1 && fragShader != -1) {
				  newProgramID = glCreateProgram();

				  glAttachShader(newProgramID, vertShader);
				  glAttachShader(newProgramID, fragShader);
				  glLinkProgram(newProgramID);

				  // Check for Linking Errors
				  char infoLog[512];
				  int success;
				  glGetProgramiv(newProgramID, GL_LINK_STATUS, &success);
				  if (!success) {
					  glGetProgramInfoLog(newProgramID, 512, NULL, infoLog);
					  std::cerr << "Program Linking ERROR: Failed to link\n" << infoLog;
				  }

				  // Success, set new ProgramID
				  else {
					  std::cout << "Program Shader Compiled Successfuly!\n";
					  programID = newProgramID;
				  }
			  }
		  }
	  }

  public:
	  App(unsigned int  width, unsigned int  height, const char* title)
		  : SimpleRender(width, height, title) {}

	  void enableLiveShaderUpdate() { shaderUpdateActive = true; }
	  void disableLiveShaderUpdate() { shaderUpdateActive = false; }


	  void Preload() {
		  // Create some Verticies
		  std::vector<Vector3> verts = {
			{ -0.4f, -0.2f, 0.0f },     // Bottom-Left
			{ -0.2f, -0.2f, 0.0f },     // Bottom-Right
			{ -0.4f,  0.2f, 0.0f },     // Top-Left
			{ -0.2f,  0.2f, 0.0f }      // Top-Right
		  };

		  GLuint indicies[] = {	
			  0, 1, 2,					// 1st Triangle
			  1, 2, 3					// 2nd Triangle
		  };


		  // Bind them
		  float* rawVerts = new float[verts.size() * 3];
		  getRawVecData(verts, rawVerts);
		  bufferData.push_back(
			  createBuffer( rawVerts, getVec3Size(verts), indicies, sizeof(indicies) )
		  );
		  delete[] rawVerts;


		  // Display some Internal Info
		  int nrAttribs;
		  glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttribs);
		  std::cout << "Maximum number of Vertex Attributes Supported: " << nrAttribs << std::endl;

	  }

	  void Draw() {
		  glUseProgram(programID);			// Use Default Program

		  float timeVal = glfwGetTime();

		  float redVal = (sin(timeVal) / 2.0f) + 0.1f;
		  float greenVal = (sin(timeVal) / 8.0f) + 0.5f;
		  float blueVal = (sin(timeVal) / 4.0f) + 0.8f;

		  GLuint colorLocation = glGetAttribLocation(programID, "rgbaColor");
		  glVertexAttrib4f(colorLocation, redVal, greenVal, blueVal, 1.0f);


		  for (BufferData& bd : bufferData) {
			  glBindVertexArray(bd.VAO);
			  glDrawElements(GL_TRIANGLES, bd.indiciesElts, GL_UNSIGNED_INT, 0);
			  glBindVertexArray(0);
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