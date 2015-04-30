#include "icg_common.h"
#include "ShadowBuffer.h"
#include "attrib_locations.h"
#include "_floor/Floor.h"
#include "_tangle_cube/TangleCube.h"

#ifdef WITH_ANTTWEAKBAR
#include <AntTweakBar.h>
TwBar *bar;
#endif

int window_width = 1024;
int window_height = 768;

ShadowBuffer sb;  // FBO for shadow map generation

// Scene elements
TangleCube tangle_cube;
Floor ground_floor;

GLuint default_pid;  // Handle for the default shader program
GLuint shadow_pid;  // Handle for the shadow map genration shader program
GLuint depth_tex;  // Handle for the shadow map

mat4 projection;  // Projection matrix for camera
mat4 view;  // View matrix for camera

vec3 light_dir;  // Direction towards the light
mat4 light_projection;  // Projection matrix for light source

mat4 offset_matrix;  // Affine transformation to map components from [-1, 1] to [0, 1], defined in init()



// TODO: Set the bias (or use AntTweakBar to modify it during runtime).
float bias = 0.05f;

#ifdef WITH_ANTTWEAKBAR
bool show_light_depth = false;
void DepthDisplaySet(const void *value, void *) {
    show_light_depth = *(const bool*)value;
    if (show_light_depth) {
      sb.display();
    } else {
      sb.unshow();
    }
}

void DepthDisplayGet(void *value, void *) {
    *(bool*)value = show_light_depth;
}
#endif

mat4 OrthographicProjection(float left, float right, float bottom, float top, float near, float far){
    assert(right > left);
    assert(far > near);
    assert(top > bottom);
    mat4 ortho = mat4::Zero();
    ortho(0, 0) = 2.0f / (right - left);
    ortho(1, 1) = 2.0f / (top - bottom);
    ortho(2, 2) = -2.0f / (far - near);
    ortho(3, 3) = 1.0f;
    ortho(0, 3) = -(right + left) / (right - left);
    ortho(1, 3) = -(top + bottom) / (top - bottom);
    ortho(2, 3) = -(far + near) / (far - near);
    return ortho;
}

void init() {
    light_dir = vec3(2.0, 4.0, -1.0);
    light_dir.normalize();

#ifdef WITH_ANTTWEAKBAR
    TwInit(TW_OPENGL_CORE, NULL);
    TwWindowSize(window_width, window_height);
    bar = TwNewBar("AntTweakBar");
    TwAddVarRW(bar, "Cube Rotation", TW_TYPE_BOOLCPP, &rotate_cube, NULL);
    TwAddVarRW(bar, "LightDir", TW_TYPE_DIR3F, &light_dir, NULL);
    TwAddVarCB(bar, "Display Light Depth", TW_TYPE_BOOLCPP, DepthDisplaySet, DepthDisplayGet, NULL, NULL);
    TwAddVarRW(bar, "Bias", TW_TYPE_FLOAT, &bias, " step=0.0001 ");

    ///--- Callbacks
    glfwSetMouseButtonCallback((GLFWmousebuttonfun)TwEventMouseButtonGLFW);
    glfwSetMousePosCallback((GLFWmouseposfun)TwEventMousePosGLFW);
    glfwSetMouseWheelCallback((GLFWmousewheelfun)TwEventMouseWheelGLFW);
    glfwSetKeyCallback((GLFWkeyfun)TwEventKeyGLFW);
    glfwSetCharCallback((GLFWcharfun)TwEventCharGLFW);
#endif

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    // Shader loading a bit hacky.
    // We have to relink the shaders after setting the attribute locations.
    // Why do we bind them instead of querying them? Because we want to
    // guarantee that we use the same locations for both, the shadow map shader
    // and the default shader.
    default_pid = opengp::load_shaders("vshader.glsl", "fshader.glsl");
    glBindAttribLocation(default_pid, ATTRIB_LOC_vpoint, "vpoint");
    glBindAttribLocation(default_pid, ATTRIB_LOC_vtexcoord, "vtexcoord");
    glLinkProgram(default_pid);

    shadow_pid = opengp::load_shaders("shadow_map_vshader.glsl", "shadow_map_fshader.glsl");
    glBindAttribLocation(shadow_pid, ATTRIB_LOC_vpoint, "vpoint");
    glLinkProgram(shadow_pid);

    glViewport(0,0,window_width,window_height);

    float ratio = window_width / (float) window_height;
    projection = Eigen::perspective(45.0f, ratio, 1.0f, 100.0f);
    view = Eigen::lookAt(vec3(2.0f, 2.0f, 2.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));

    glUseProgram(default_pid);
    glUniformMatrix4fv(glGetUniformLocation(default_pid, "projection"), 1, GL_FALSE, projection.data());
    glUniformMatrix4fv(glGetUniformLocation(default_pid, "view"), 1, GL_FALSE, view.data());
    glUniform1i(glGetUniformLocation(default_pid, "shadow_map"), 1);

    // TODO: Setup light projection matrix.
    // Use orthographic projection for directional light.
    // Define frustum that tightly encloses the rendered scene.
    // (The rendered scene lives approximatly in a cube from [-1, -1, 1] to [1, 1, -1].)
    light_projection = OrthographicProjection(-1.5,1.5,-1.5,1.5,-0.5,2.5);

    // Matrix that can be used to move a point's components from [-1, 1] to [0, 1].
    offset_matrix << 0.5f, 0.0f, 0.0f, 0.5f,
                     0.0f, 0.5f, 0.0f, 0.5f,
                     0.0f, 0.0f, 0.5f, 0.5f,
                     0.0f, 0.0f, 0.0f, 1.0f;

    check_error_gl();

    tangle_cube.init();
    depth_tex = sb.init();
    ground_floor.init();
}

void display() {
    glUseProgram(shadow_pid);
    //=== BIND
    sb.bind();

    // TODO: Setup light view matrix. You can use the lookAt function.
    mat4 light_view = Eigen::lookAt(light_dir,vec3(0,0,0),vec3(0,1,0));// mat4::Zero(4, 4);

    mat4 depth_vp = light_projection * light_view;
    glUniformMatrix4fv(glGetUniformLocation(shadow_pid, "depth_vp"), 1, GL_FALSE, depth_vp.data());

    glClear(GL_DEPTH_BUFFER_BIT);
    ground_floor.draw(shadow_pid);
    tangle_cube.draw(shadow_pid);
    sb.unbind();
    //=== UNBIND

    glUseProgram(default_pid);
    glUniform3fv(glGetUniformLocation(default_pid, "light_dir"), 1, light_dir.data());

    // Set matrix to transform from world space into NDC and then into [0, 1] ranges.
    mat4 depth_vp_offset = offset_matrix * depth_vp;
    glUniformMatrix4fv(glGetUniformLocation(default_pid, "depth_vp_offset"), 1, GL_FALSE, depth_vp_offset.data());

    glUniform1f(glGetUniformLocation(default_pid, "bias"), bias);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depth_tex);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ground_floor.draw(default_pid);
    tangle_cube.draw(default_pid);

    glIsQuery(0);

#ifdef WITH_ANTTWEAKBAR
    TwDraw();
#endif
}

int main(int, char**) {
    glfwInitWindowSize(window_width, window_height);
    glfwCreateWindow("shadow mapping");
    glfwDisplayFunc(display);
    init();
    glfwMainLoop();

#ifdef WITH_ANTTWEAKBAR
    TwTerminate();
#endif

    return EXIT_SUCCESS;    
}
