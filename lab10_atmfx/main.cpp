#include "icg_common.h"
#include <stdio.h>
#include <math.h>
#include "quad.h"
#define WIDTH   800
#define HEIGHT  600
using namespace std;

Quad::Parameters pars;
Quad screen_quad;
#ifdef WITH_ANTTWEAKBAR
    #include <AntTweakBar.h>
    TwBar * bar = NULL;
#endif

void init (){
    glfwSwapInterval(0); ///< disable VSYNC (allows framerate>30)
    screen_quad.init(WIDTH, HEIGHT);
    
#ifdef WITH_ANTTWEAKBAR
    ///--- Init
    TwInit (TW_OPENGL_CORE, NULL);
    TwWindowSize (WIDTH, HEIGHT);
    bar = TwNewBar ("AntTweakBar");
    ///--- Setup    
    TwAddVarRW (bar, "LightDir", TW_TYPE_DIR3F, &pars.light_dir, NULL);
    TwAddVarRW (bar, "K_ray", TW_TYPE_COLOR3F, &pars.K_ray, NULL); 
    TwAddVarRW (bar, "K_mie", TW_TYPE_FLOAT, &pars.K_mie, "min=0.00000 max=1.0 step=0.00001");
    TwAddVarRW (bar, "I_sun", TW_TYPE_FLOAT, &pars.I_sun, NULL);
    TwAddVarRW (bar, "H_0", TW_TYPE_FLOAT, &pars.H_0, "min=0.0 max=1.0 step=0.1");
    TwAddVarRW (bar, "InnerRadius", TW_TYPE_FLOAT, &pars.inner_radius, "min=0.0 max=10000 step=1");
    TwAddVarRW (bar, "OuterRadius", TW_TYPE_FLOAT, &pars.outer_radius, "min=0.0 max=10000 step=1");
    TwAddVarRW (bar, "Camera height", TW_TYPE_FLOAT, &pars.camera_height, "min=0.00000001 max=100.0 step=1.0");
    TwAddVarRW (bar, "Camera angle", TW_TYPE_FLOAT, &pars.cam_angle, "min=-1.57 max=1.57 step=0.01");
    TwAddVarRW (bar, "Integration steps", TW_TYPE_UINT8, &pars.nb_samples, "min=1 max=20 step=1");
    ///--- Callbacks
    glfwSetKeyCallback ((GLFWkeyfun)TwEventKeyGLFW);
    glfwSetCharCallback ((GLFWcharfun)TwEventCharGLFW);
    glfwSetMouseWheelCallback ((GLFWmousewheelfun)TwEventMouseWheelGLFW);
    glfwSetMouseButtonCallback ((GLFWmousebuttonfun)TwEventMouseButtonGLFW);
    glfwSetMousePosCallback ((GLFWmouseposfun)TwEventMousePosGLFW);
#endif
}

void cleanup(){
    screen_quad.cleanup();
#ifdef WITH_ANTTWEAKBAR
    TwTerminate();
#endif
}

mat4 camera_matrix(float angle, float height){
    float c = cos (-angle);
    float s = sin (-angle);
    mat4 t = mat4::Identity();
    t(1, 3) = -height;
    mat4 r = mat4::Zero();
    r(0, 0) = 1;
    r(1, 1) = c;
    r(2, 1) = s;
    r(1, 2) = -s;
    r(2, 2) = c;
    r(3, 3) = 1;
    return t * r;
}

void display(){
    opengp::update_title_fps("atmfx"); ///< ~300 FPS on macbook pro retina @ 1920*1080
    mat4 M = camera_matrix(pars.cam_angle, pars.camera_height);
    screen_quad.draw(M, pars);
#ifdef WITH_ANTTWEAKBAR
    TwDraw();
#endif
}

int main (int, char**){
    glfwInitWindowSize(WIDTH, HEIGHT);
    glfwCreateWindow("atmfx");
    glfwDisplayFunc(display);
    init();
    glfwMainLoop();
    cleanup();
    return EXIT_SUCCESS;
}

