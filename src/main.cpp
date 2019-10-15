#include "includes/SimpleRender.h"
#include "includes/Vector.h"

// OpenGL Macros
#define WIDTH 800
#define HEIGHT 400


class App : public SimpleRender {
  private:


  public:
	  App(unsigned int  width, unsigned int  height, const char* title)
		  : SimpleRender(width, height, title) {}


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

	  //void Draw() {}
};




int main() {
	//SimpleRender render(640, 480, "triangle opengl");
	//int status = render.run();
	//if (status != 0)
	//	std::cerr << "status = " << status << std::endl;

	App app(WIDTH, HEIGHT, "Triangle OpenGL");
	int status = app.run();
	if (status != 0)
        std::cerr << "Status = " << status << std::endl;

    return 0;
}