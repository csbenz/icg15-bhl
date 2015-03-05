#include "icg_common.h"
#include "cube.h"
#include "_quad/Quad.h"

using namespace std;

Cube cube;
Quad quad;

int WIDTH = 800;
int HEIGHT = 600;

mat4 projection_matrix;
mat4 view_matrix;

mat4 OrthographicProjection(float left, float right, float bottom, float top, float near, float far){
    assert(right > left);
    assert(far > near);
    assert(top > bottom);
    mat4 ortho = mat4::Zero();
    ortho(0, 0) = 2.0f / (right - left);
    ortho(1, 1) = 2.0f / (top - bottom);
    ortho(2, 2) = -2.0f / (far - near);
    ortho(3, 3) = 1.0f;
    ortho(1, 3) = -(right + left) / (right - left);
    ortho(2, 3) = -(top + bottom) / (top - bottom);
    ortho(2, 3) = -(far + near) / (far - near);
    return ortho;
}

mat4 LookAt(vec3 eye, vec3 center, vec3 up) {
    // We need a function that converts from world coordinates into camera coordiantes.
    //
    // Cam coords to world coords is given by:
    // X_world = R * X_cam + eye
    //
    // inverting it leads to:
    //
    // X_cam = R^T * X_world - R^T * eye
    //
    // Or as a homogeneous matrix:
    // [ r_00 r_10 r_20 -r_0*eye
    //   r_01 r_11 r_21 -r_1*eye
    //   r_02 r_12 r_22 -r_2*eye
    //      0    0    0        1 ]

    vec3 z_cam = (eye - center).normalized();
    vec3 x_cam = up.cross(z_cam).normalized();
    vec3 y_cam = z_cam.cross(x_cam);

    mat3 R;
    R.col(0) = x_cam;
    R.col(1) = y_cam;
    R.col(2) = z_cam;

    mat4 look_at = mat4::Zero();
    look_at.block(0, 0, 3, 3) = R.transpose();
    look_at(3, 3) = 1.0f;
    look_at.block(0, 3, 3, 1) = -R.transpose() * (eye);
    return look_at;
}

// Gets called when the windows is resized.
void resize_callback(int width, int height) {
    WIDTH = width;
    HEIGHT = height;

    std::cout << "Window has been resized to " << WIDTH << "x" << HEIGHT << "." << std::endl;
    glViewport(0, 0, WIDTH, HEIGHT);

    GLfloat top = 1.0f;
    GLfloat right = (GLfloat)WIDTH / HEIGHT * top;
    projection_matrix = OrthographicProjection(-right, right, -top, top, -10.0, 10.0f);
}

void init(){
    // Sets background color.
    glClearColor(/*gray*/ .937,.937,.937, /*solid*/1.0);
    
    cube.init();
    quad.init();

    // Enable depth test.
    glEnable(GL_DEPTH_TEST);

    view_matrix = LookAt(vec3(2.0f, 2.0f, 4.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
}

// Gets called for every frame.
void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Scaling matrix to scale the cube down to a reasonable size.
    mat4 cube_scale;
    cube_scale << 0.25f, 0.0f,  0.0f,  0.0f,
                  0.0f,  0.25f, 0.0f,  0.0f,
                  0.0f,  0.0f,  0.25f, 0.0f,
                  0.0f,  0.0f,  0.0f,  1.0f;
    const float time = glfwGetTime();
    mat4 cube_trafo = (Eigen::Affine3f(Eigen::AngleAxisf(2.0f * time, vec3::UnitY())) *
                       Eigen::Affine3f(Eigen::Translation3f(vec3(0.75f, 0.0f, 0.0f))) *
                       Eigen::Affine3f(Eigen::AngleAxisf(2.0f * time, vec3::UnitY()))).matrix();
    //mat4 cube_trafo = Eigen::Affine3f(Eigen::AngleAxisf(time, vec3::UnitY())).matrix();
    mat4 cube_model_matrix = cube_trafo * cube_scale;

    cube.draw(cube_model_matrix, view_matrix, projection_matrix);

    // Draw a quad on the ground.
    mat4 quad_model_matrix = (Eigen::Affine3f(Eigen::Translation3f(vec3(0.0f, -0.25f, 0.0f))) *
                              Eigen::Affine3f(Eigen::AngleAxisf(M_PI * 0.5f, -vec3::UnitX()))).matrix();
    quad.draw(quad_model_matrix, view_matrix, projection_matrix);

    check_error_gl();
}

int main(int, char**){
    glfwInitWindowSize(WIDTH, HEIGHT);
    glfwCreateWindow("3D Transformations");
    glfwDisplayFunc(display);
    glfwSetWindowSizeCallback(&resize_callback);
    init();
    glfwMainLoop();
    return EXIT_SUCCESS;
}
