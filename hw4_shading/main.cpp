#include "icg_common.h"
#include "_quad/Quad.h"
#include "_mesh/Mesh.h"

#ifdef WITH_ANTTWEAKBAR
#include <AntTweakBar.h>
TwBar *bar;
#endif

int window_width = 1024;
int window_height = 768;

Quad quad;
Mesh mesh;

void init(){
    glClearColor(1,1,1, /*solid*/1.0 );    
    glEnable(GL_DEPTH_TEST);
    quad.init();
    mesh.init("tangle_cube.obj");

#ifdef WITH_ANTTWEAKBAR
    TwInit(TW_OPENGL_CORE, NULL);
    TwWindowSize(window_width, window_height);
    bar = TwNewBar("AntTweakBar");
    ///--- Light
    TwAddVarRW(bar, "Ia", TW_TYPE_COLOR3F, mesh.Ia.data(), NULL);
    TwAddVarRW(bar, "Id", TW_TYPE_COLOR3F, mesh.Id.data(), NULL);
    TwAddVarRW(bar, "Is", TW_TYPE_COLOR3F, mesh.Is.data(), NULL);
    TwAddVarRW(bar, "LightDir", TW_TYPE_DIR3F, &mesh.light_pos, NULL);
    ///--- Material
    TwAddVarRW(bar, "Ka", TW_TYPE_COLOR3F, mesh.ka.data(), NULL);
    TwAddVarRW(bar, "Kd", TW_TYPE_COLOR3F, mesh.kd.data(), NULL);
    TwAddVarRW(bar, "Ks", TW_TYPE_COLOR3F, mesh.ks.data(), NULL);
    TwAddVarRW(bar, "p", TW_TYPE_FLOAT, &mesh.p, "min=.5 max=50 step=0.4");
    ///--- Callbacks
    ///--- https://github.com/davidcox/AntTweakBar/blob/master/examples/TwSimpleGLFW.c
    glfwSetMouseButtonCallback((GLFWmousebuttonfun)TwEventMouseButtonGLFW);
    glfwSetMouseButtonCallback((GLFWmousebuttonfun)TwEventMouseButtonGLFW);
    glfwSetMousePosCallback((GLFWmouseposfun)TwEventMousePosGLFW);
    glfwSetMouseWheelCallback((GLFWmousewheelfun)TwEventMouseWheelGLFW);
    // glfwSetKeyCallback((GLFWkeyfun)TwEventKeyGLFW);
    // glfwSetCharCallback((GLFWcharfun)TwEventCharGLFW);
#endif
}

void display(){
    opengp::update_title_fps("shading");   
    glViewport(0,0,window_width,window_height);
    
    ///--- Setup view-projection matrix
    float ratio = window_width / (float) window_height;
    static mat4 projection = Eigen::perspective(45.0f, ratio, 0.1f, 10.0f);
    vec3 cam_pos(2.0f, 2.0f, 2.0f);
    vec3 cam_look(0.0f, 0.0f, 0.0f);
    vec3 cam_up(0.0f, 0.0f, 1.0f);
    mat4 view = Eigen::lookAt(cam_pos, cam_look, cam_up);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    quad.draw(mat4::Identity(), view, projection);
    mesh.draw(mat4::Identity(), view, projection);
    
#ifdef WITH_ANTTWEAKBAR
    TwDraw();
#endif
}

void cleanup(){
#ifdef WITH_ANTTWEAKBAR
    TwTerminate();
#endif
}

void keyboard(int key, int action){
    if(action != GLFW_RELEASE) return; ///< only act on release
    switch(key){
        case '0':
            std::cout << "gouraud shading" << std::endl;
            mesh.use_gouraud_shader();
        break;
        case '1':
            std::cout << "phong shading" << std::endl;
            mesh.use_phong_shader();
            break;
        case '2':
            std::cout << "flat shading" << std::endl;
            mesh.use_flat_shader();
            break;
        case '3':
            std::cout << "toon shading" << std::endl;
            mesh.use_toon_shader();
            break;
        case '4':
            std::cout << "artistic shading" << std::endl;
            mesh.use_artistic_shader();
            break;
        case '5':
            std::cout << "spot light phong shading" << std::endl;
            mesh.use_spot_light_phong_shader();
            break;
        default:
            break;
    }
}

int main(int, char**){
    glfwInitWindowSize(window_width, window_height);
    glfwCreateWindow();
    glfwDisplayFunc(display);
    glfwSetKeyCallback(keyboard);
    init();
    keyboard(GLFW_KEY_KP_1, 0);
    glfwMainLoop();
    cleanup();
    return EXIT_SUCCESS;
}
