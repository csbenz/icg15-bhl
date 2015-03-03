#include "icg_common.h"
#include "Quad.h"

Quad quad;

void init()
{
    glClearColor(/*gray*/ .937,.937,.937, /*solid*/ 1.0);
    quad.init();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    float time_s = glfwGetTime();
    quad.draw(time_s);
}

int main(int, char**)
{
    glfwInitWindowSize(512, 512);
    glfwCreateWindow("checkerboard");
    glfwDisplayFunc(display);
    init();
    glfwMainLoop();
    quad.cleanup();
    return EXIT_SUCCESS;
}
