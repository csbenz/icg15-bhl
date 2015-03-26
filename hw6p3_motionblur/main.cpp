#include "icg_common.h"
#include "Wheel.h"
#include "FrameBuffer.h"

typedef Eigen::Transform<float,3,Eigen::Affine> Transform;

int window_width = 1024;
int window_height = 768;
FrameBuffer fb(window_width, window_height);

Wheel _quad;
mat4 _projection;
mat4 _view;
mat4 _old_MVP;

float _angle = 0.0;
float _previousTime = 0.0;
float _spin_factor = 3;
bool enabled = true;

void init() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    fb.init(true /*interpolate*/);
    _quad.init();

    float ratio = window_width / (float) window_height;
    _projection = Eigen::perspective(45.0f, ratio, 0.1f, 10.0f);
    vec3 cam_pos(0.0f, 0.01f, 3.0f);
    vec3 cam_look(0.0f, 0.0f, 0.0f);
    vec3 cam_up(0.0f, 0.0f, 1.0f);
    _view = Eigen::lookAt(cam_pos, cam_look, cam_up);
    _old_MVP = _projection * _view;
}

void display() {
    // The Window freeze eachtime update_title_fps is called, desactivate it for now
    opengp::update_title_fps("Motion Blur");   
    glViewport(0,0,window_width,window_height);
   
    Transform model = Transform::Identity();
    
    float currentTime = glfwGetTime();
    float dt = currentTime - _previousTime;
    _previousTime = currentTime;
    _angle += dt*_spin_factor;

    model *= Eigen::AngleAxisf(-_angle, Eigen::Vector3f::UnitZ());
    mat4 MVP = _projection * _view * model.matrix();
       
    fb.clear();
    fb.bind();
        _quad.draw(MVP, enabled?_old_MVP:MVP);
    fb.unbind();
    // fb.display_velocity_attachment("velocity");
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    fb.draw();
    _old_MVP = MVP;
}

void keyboard(int key, int action) {
    if(action == GLFW_RELEASE) {
        switch (key) {
            case 'Q': _spin_factor+=0.33; break;
            case 'A': _spin_factor-=0.33; break;
            case 'E': enabled = !enabled; break;
            default: break;
        }
        std::cout<< "_spin_factor "<< _spin_factor << std::endl;
    }
}

int main(int, char**) {
    glfwInitWindowSize(window_width, window_height);
    glfwCreateWindow("Motion Blur");
    glfwDisplayFunc(display);
    glfwSetKeyCallback(keyboard);
    // glfwSwapInterval(0); ///< disable VSYNC (allows framerate>30)
    init();
    glfwMainLoop();
    return EXIT_SUCCESS;    
}
