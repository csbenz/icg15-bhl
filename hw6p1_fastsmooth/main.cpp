#include "icg_common.h"
#include "FrameBuffer.h"
#include "_cube/Cube.h"
#include "_quad/Quad.h"
#include "_screenquad/ScreenQuad.h"

int width=1280, height=720;

Cube cube;
Quad quad;

FrameBuffer fb(width, height);
FrameBuffer fb_filter(width, height);
ScreenQuad squad;

void init(){
    glClearColor(1,1,1, /*solid*/1.0 );    
    glEnable(GL_DEPTH_TEST);
    cube.init();
    quad.init();
    GLuint tex_scene = fb.init();
    GLuint tex_filter = fb_filter.init();
    squad.init(tex_scene, tex_filter);
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

    ///--- Render to FB
    fb.bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        cube.draw(VP, glfwGetTime());
        quad.draw(VP);
    fb.unbind();

    // Stocking to temp_tex
    fb_filter.bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        squad.draw(0);
    fb_filter.unbind();

    ///--- Render to Window
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    squad.draw(1);
}
void keyboard(int key, int action){
    if(action != GLFW_RELEASE) return; ///< only act on release
    switch(key){
        case '1':
            squad.changeMode(0);
            std::cout << "BRUTEFORCE" << std::endl << std::flush;
            break;
        case '2':
            squad.changeMode(1);
            std::cout << "OPTIMIZED" << std::endl << std::flush;
            break;
        case 'Q':
            squad.decStd();
            break;
        case 'W':
            squad.incStd();
            break;
        default:
            break;
    }
}

int main(int, char**){
    glfwInitWindowSize(width, height);
    glfwCreateWindow();
    glfwDisplayFunc(display);
    glfwSetKeyCallback(keyboard);
    init();
    glfwSwapInterval(0); ///< disable VSYNC (allows framerate>30)
    glfwMainLoop();
    return EXIT_SUCCESS;
}
