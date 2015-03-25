#include "icg_common.h"
#include "FrameBuffer.h"
#include "_cube/Cube.h"
#include "_quad/Quad.h"
#include "_screenquad/ScreenQuad.h"

int width=1280, height=720;

Cube cube;
Quad quad;

// TODO: declare Framebuffer + ScreenQuad (see slides)

void init(){
    glClearColor(1,1,1, /*solid*/1.0 );    
    glEnable(GL_DEPTH_TEST);
    cube.init();
    quad.init();
    // TODO: initialize framebuffer (see slides)
    // TODO: initialize fullscreen quad (see slides)
}

void display(){ 
    opengp::update_title_fps("FrameBuffer");   
    glViewport(0,0,width,height);
    
    ///--- Setup view-projection matrix
    float ratio = width / (float) height;
    static mat4 projection = Eigen::perspective(45.0f, ratio, 0.1f, 10.0f);
    vec3 cam_pos(2.0f, 2.0f, 2.0f);
    vec3 cam_look(0.0f, 0.0f, 0.0f);
    vec3 cam_up(0.0f, 0.0f, 1.0f);
    mat4 view = Eigen::lookAt(cam_pos, cam_look, cam_up);
    mat4 VP = projection * view;
    
    // TODO: wrap these calls so they render to a texture (see slides)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    cube.draw(VP, glfwGetTime());
    quad.draw(VP);
    
    // TODO: use the fullscreen quad to draw the FB texture to screen (see slides)
}

int main(int, char**){
    glfwInitWindowSize(width, height);
    glfwCreateWindow();
    glfwDisplayFunc(display);
    init();
    glfwSwapInterval(0); ///< disable VSYNC (allows framerate>30)
    glfwMainLoop();
    return EXIT_SUCCESS;
}
