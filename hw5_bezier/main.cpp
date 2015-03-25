#include "icg_common.h"
#include "trackball.h"

#include "_mesh/Mesh.h"
#include "_quad/Quad.h"
#include "_cube/cube.h"
#include "_point/point.h"
#include "_bezier/bezier.h"

enum NAVIGATION_MODE {
    TRACKBALL,
    BEZIER
} navmode;

int window_width = 1024;
int window_height = 768;

mat4 projection;
mat4 view;
mat4 model;

GLuint _pid_bezier;
GLuint _pid_point;
GLuint _pid_point_selection;

///----------------- Trackball -----------------
mat4 trackball_matrix;
mat4 old_trackball_matrix;
Trackball trackball;

vec2 transform_screen_coords(int x, int y) {
    return vec2(2.0f * (float)x / window_width - 1.0f,
                1.0f - 2.0f * (float)y / window_height);
}

void trackball_button(int button, int action) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        int x_i, y_i;
        glfwGetMousePos(&x_i, &y_i);
        vec2 p = transform_screen_coords(x_i, y_i);
        trackball.begin_drag(p.x(), p.y());
        old_trackball_matrix = trackball_matrix;  // Store the current state of the model matrix.
    }
}

void trackball_pos(int x, int y) {
    if (glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        vec2 p = transform_screen_coords(x, y);
        trackball_matrix = trackball.drag(p.x(), p.y()) * old_trackball_matrix;
    }
}
///---------------------------------------------------
Quad quad;

BezierCurve cam_pos_curve;
BezierCurve cam_look_curve;
Cube cube;
/// We tried to add a mesh but the thing doesn't appear the way it should..
/// And no time to correct it so we give it like that.
Mesh mesh;

std::vector<ControlPoint> cam_pos_points;
std::vector<ControlPoint> cam_look_points;
int selected_point;

int starting_time = 0;
int moving_time = 10;

void init(){
    /// Compile the shaders here to avoid the duplication
    _pid_bezier = opengp::load_shaders("bezier_vshader.glsl", "bezier_fshader.glsl");
    if(!_pid_bezier) exit(EXIT_FAILURE);

    _pid_point = opengp::load_shaders("point_vshader.glsl", "point_fshader.glsl");
    if(!_pid_point) exit(EXIT_FAILURE);

    _pid_point_selection = opengp::load_shaders("point_selection_vshader.glsl", "point_selection_fshader.glsl");
    if(!_pid_point_selection) exit(EXIT_FAILURE);

    glClearColor(1,1,1, /*solid*/1.0 );    
    glEnable(GL_DEPTH_TEST);
    quad.init();
    cube.init();
    mesh.init("tangle_cube.obj");

    ///--- init cam_pos_curve
    cam_pos_curve.init(_pid_bezier);
    cam_pos_points.push_back(ControlPoint(-1.18, -0.3, 0.94, 0));
    cam_pos_points.push_back(ControlPoint(-0.6, -1.5, 0.68, 1));
    cam_pos_points.push_back(ControlPoint(0.95, -0.5, 0.3, 2));
    cam_pos_points.push_back(ControlPoint(1.01, 0.15, 0.23, 3));
    cam_pos_points.push_back(ControlPoint(1.07, 0.8, 0.1, 4));
    cam_pos_points.push_back(ControlPoint(-0.11, 0.94, -0.09, 5));
    cam_pos_points.push_back(ControlPoint(-0.61, 0.84, 0.46, 6));
    cam_pos_points.push_back(ControlPoint(-0.5, 0.70, 1.08, 7));
    cam_pos_points.push_back(ControlPoint(-1.76, -0.08, 1.2, 8));
    cam_pos_points.push_back(ControlPoint(-1.18, -0.3, 0.94, 9));

    for (unsigned int i = 0; i < cam_pos_points.size(); i++) {
        cam_pos_points[i].init(_pid_point, _pid_point_selection);
    }

    cam_pos_curve.set_points(cam_pos_points);

    ///--- init cam_look_curve
    cam_look_curve.init(_pid_bezier);

    ///===================== TODO =====================
    ///--- TODO H5.3: Set points for cam_look_curve here
    /// Don't forget to set correct point ids.
    /// ===============================================

    /// We can change those values to make it less smooth
    /// We did a loop so it's 'smoothy'
    cam_look_points.push_back(ControlPoint(-0.25, -0.25, 0.25, 10));
    cam_look_points.push_back(ControlPoint(0.25, -0.25, 0.25, 11));
    cam_look_points.push_back(ControlPoint(0.25, 0.25, 0.25, 12));
    cam_look_points.push_back(ControlPoint(-0.25, -0.25, 0.25, 13));

    for (unsigned int i = 0; i < cam_look_points.size(); i++) {
        cam_look_points[i].init(_pid_point, _pid_point_selection);
    }

    cam_look_curve.set_points(cam_look_points);

    ///--- Setup view-projection matrix
    float ratio = window_width / (float) window_height;
    projection = Eigen::perspective(45.0f, ratio, 0.1f, 10.0f);

    vec3 cam_pos(2.0f, 2.0f, 2.0f);
    vec3 cam_look(0.0f, 0.0f, 0.0f);
    vec3 cam_up(0.0f, 0.0f, 1.0f);

    view = Eigen::lookAt(cam_pos, cam_look, cam_up);
    model = mat4::Identity();
    trackball_matrix = mat4::Identity();
    selected_point = -1;
    navmode = TRACKBALL;
}

bool unproject (int mouse_x, int mouse_y, vec3 &p) {

    mat4 MVP = (projection * view * trackball_matrix * model);

    vec4 p_screen = MVP * vec4(p[0], p[1], p[2], 1.0f);

    vec4 mouse_pos = vec4(2.0f * (float)mouse_x / window_width - 1.0f,
                    1.0f - 2.0f * (float)mouse_y / window_height, p_screen[2]/p_screen[3], 1.0f);

    mat4 MVP_inv = (MVP).inverse();

    p_screen = MVP_inv * mouse_pos;

    float w = p_screen[3];

    p = vec3(p_screen[0] / w, p_screen[1] / w, p_screen[2] / w);

    return true;
}

void display(){
    opengp::update_title_fps("shading");   
    glViewport(0,0,window_width,window_height);
        
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (navmode == BEZIER) {
        vec3 cam_pos(2.0f, 2.0f, 2.0f);
        vec3 cam_look(0.0f, 0.0f, 0.0f);
        vec3 cam_up(0.0f, 0.0f, 1.0f);

        ///===================== TODO =====================
        ///--- TODO H5.3 sample cam_pos and cam_up from cam_pos_curve
        /// and cam_look_curve.
        /// Use glfwGetTime() and wrap it to [0, 1] to get the curve
        /// parameter.
        ///================================================

        float time = (glfwGetTime() - starting_time) / moving_time;

        if (time >= 1) {
            starting_time = glfwGetTime();
            time = time - 1;
        }

        cam_pos_curve.sample_point(time, cam_pos);
        cam_look_curve.sample_point(time, cam_look);

        mat4 view_bezier = Eigen::lookAt(cam_pos, cam_look, cam_up);
        quad.draw(model, view_bezier, projection);
        cube.draw(model, view_bezier, projection);
    }

    if (navmode == TRACKBALL) {
        for (unsigned int i = 0; i < cam_pos_points.size(); i++) {
            cam_pos_points[i].draw(trackball_matrix * model, view, projection);
        }


        ///===================== TODO =====================
        ///--- TODO H5.3: Draw control points for cam_look_curve
        /// ===============================================
        for (unsigned int i = 0; i < cam_look_points.size(); i++) {
            cam_look_points[i].draw(trackball_matrix * model, view, projection);
        }

        cam_pos_curve.draw(trackball_matrix * model, view, projection);
        cam_look_curve.draw(trackball_matrix * model, view, projection);

        quad.draw(trackball_matrix * model, view, projection);
        cube.draw(trackball_matrix * model, view, projection);
    }
}

void render_selection() {
    glViewport(0,0,window_width,window_height);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    ///===================== TODO =====================
    ///--- TODO P5.2 Draw control points for selection
    ///================================================

    for (unsigned int i = 0; i < cam_pos_points.size(); i++) {
        cam_pos_points[i].draw_selection(trackball_matrix * model, view, projection);
    }

    ///===================== TODO =====================
    ///--- TODO H5.3 Draw control points for cam_look_curve
    ///================================================
    for (unsigned int i = 0; i < cam_look_points.size(); i++) {
        cam_look_points[i].draw_selection(trackball_matrix * model, view, projection);
    }
}

void selection_button(int button, int action) {
    static int x_pressed = 0, y_pressed = 0;

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        int x = 0, y = 0;
        glfwGetMousePos(&x, &y);
        x_pressed = x; y_pressed = y;

        render_selection();

        glFlush();
        glFinish();

        unsigned char res[4];
        glReadPixels(x, window_height - y, 1,1,GL_RGBA, GL_UNSIGNED_BYTE, &res);
        selected_point = res[0];

        if (selected_point >= 0 && selected_point < cam_pos_points.size())
            cam_pos_points[selected_point].selected() = true;
        ///===================== TODO =====================
        ///--- TODO H5.3 Process cam_look_points for selection
        ///================================================
        if (selected_point >= cam_pos_points.size() && selected_point < cam_pos_points.size() + cam_look_points.size()) {
            cam_look_points[selected_point-cam_pos_points.size()].selected() = true;
        }
    }

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        if (selected_point >= 0 && selected_point < cam_pos_points.size()) {
            cam_pos_points[selected_point].selected() = false;
        }
        if (selected_point >= cam_pos_points.size() && selected_point < cam_pos_points.size() + cam_look_points.size()) {
            cam_look_points[selected_point-cam_pos_points.size()].selected() = false;
        }

        ///===================== TODO =====================
        ///--- TODO H5.3 Process cam_look_points for selection
        ///================================================
        int x = 0, y = 0;
        glfwGetMousePos(&x, &y);
        if (x == x_pressed && y == y_pressed) {
            return;
        }

        if (selected_point >= 0 && selected_point < cam_pos_points.size()) {
            unproject(x, y, cam_pos_points[selected_point].position());

            cam_pos_curve.set_points(cam_pos_points);
        }
        ///===================== TODO =====================
        ///--- TODO H5.3 Update control points of cam_look_curve
        ///================================================
        if (selected_point >= cam_pos_points.size() && selected_point < cam_pos_points.size() + cam_look_points.size()) {
            unproject(x, y, cam_look_points[selected_point-cam_pos_points.size()].position());

            cam_look_curve.set_points(cam_look_points);
        }
    }
}

void keyboard(int key, int action){
    if(action != GLFW_RELEASE) return; ///< only act on release
    switch(key){
        case '1':
            navmode = TRACKBALL;
            std::cout << "Trackball mode.\n" << std::flush;
            break;
        case '2':
            navmode = BEZIER;
            std::cout << "Bezier mode.\n" << std::flush;
            break;
        default:
            break;
    }
}

void mouse_button(int button, int action) {
    if (navmode == BEZIER) {
        std::cout << "No mouse interaction in Bezier mode.\n" << std::flush;
        return;
    }

    if (glfwGetKey(GLFW_KEY_LSHIFT) == GLFW_PRESS) {
        trackball_button(button, action);
    } else {
        selection_button(button, action);
    }
}

void mouse_pos(int x, int y) {
    if (navmode == BEZIER) {
        return;
    }
    if (glfwGetKey(GLFW_KEY_LSHIFT) == GLFW_PRESS) {
        trackball_pos(x, y);
    }
}
void cleanup(){
    glDeleteProgram(_pid_bezier);
    glDeleteProgram(_pid_point);
    glDeleteProgram(_pid_point_selection);
}

int main(int, char**){
    glfwInitWindowSize(window_width, window_height);
    glfwCreateWindow();
    glfwDisplayFunc(display);
    glfwSetKeyCallback(keyboard);
    glfwSetMouseButtonCallback(mouse_button);
    glfwSetMousePosCallback(mouse_pos);
    init();
    keyboard(GLFW_KEY_KP_1, 0);
    glfwMainLoop();
    cleanup();
    return EXIT_SUCCESS;
}
