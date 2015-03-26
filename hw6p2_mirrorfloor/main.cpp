#include "icg_common.h"
#include "FrameBuffer.h"
#include "_floor/Floor.h"
#include "_cube/Cube.h"

int window_width = 1280;
int window_height = 720;
FrameBuffer fb(window_width, window_height);
Cube cube;
Floor shinyfloor;

void init() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    cube.init();    
    // TODO: initialize framebuffer
    // TODO: initialize shinyfloor with the FB texture
    shinyfloor.init(/*???*/);
}

void display() {
    glViewport(0,0,window_width,window_height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float ratio = window_width / (float) window_height;
    static mat4 projection = Eigen::perspective(45.0f, ratio, 0.1f, 10.0f);
    vec3 cam_pos(2.0f, 2.0f, 2.0f);
    vec3 cam_look(0.0f, 0.0f, 0.0f);
    vec3 cam_up(0.0f, 0.0f, 1.0f);
    mat4 view = Eigen::lookAt(cam_pos, cam_look, cam_up);
    mat4 VP = projection * view;

    // TODO: mirror the camera position
    // TODO: create new VP for mirrored camera
    // TODO: render the cube using the mirrored camera
    // HINT: this render will be done in the framebuffer texture (remember bind/unbind)
    
    shinyfloor.draw(VP);
    cube.draw(VP, glfwGetTime()); 
}

int main(int, char**) {
    glfwInitWindowSize(window_width, window_height);
    glfwCreateWindow("mirror_floor");
    glfwDisplayFunc(display);
    init();
    glfwMainLoop();
    return EXIT_SUCCESS;    
}
