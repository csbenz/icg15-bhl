#include "icg_common.h"
#include "Triangle.h"
Triangle triangle;

typedef Eigen::Transform<float,3,Eigen::Affine> Transform;

void init(){
    glClearColor(/*gray*/ .937,.937,.937, /*solid*/1.0 );
    triangle.init();
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT);

    double golden_angle =  3.1415 * (3 - sqrt(5));

    double scale = 0.02;

    int N = 400;
    for (double i = 0; i < N; ++i) {

        //scale *= 1.0;
        double alpha = - (i * golden_angle);
        double r = sqrt(i) / sqrt(N);

         Transform _M = Transform::Identity();

        _M *= Eigen::Translation3f(r * cos(alpha), r * sin(alpha), 0);
        _M *= Eigen::AngleAxisf(alpha, Eigen::Vector3f::UnitZ());
        _M *= Eigen::AlignedScaling3f(scale, scale, scale);

        mat4 M = _M.matrix();

        triangle.draw(M); // mat4::Identity()
    }

}

int main(int, char**){
    glfwInitWindowSize(512, 512);
    glfwCreateWindow("spiral");
    glfwDisplayFunc(display);
    init();
    glfwMainLoop();
    triangle.cleanup();
    return EXIT_SUCCESS;
}

