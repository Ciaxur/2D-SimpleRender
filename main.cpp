#include "includes/SimpleRender.h"

// OpenGL Macros
#define WIDTH 800
#define HEIGHT 400


int main() {
    SimpleRender render(640, 480, "Triangle OpenGL");
    int status = render.run();
    if (status != 0) std::cerr << "Status = " << status << std::endl;

    return 0;
}