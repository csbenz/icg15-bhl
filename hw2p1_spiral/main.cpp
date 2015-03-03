#include "icg_common.h"
#include "Triangle.h"

#define USE_SPIRAL 0


Triangle triangle;

typedef Eigen::Transform<float,3,Eigen::Affine> Transform;

void spiral () {
    glClear(GL_COLOR_BUFFER_BIT);
    int numTriangles = 60;

    for (int i = 0; i < numTriangles; i++) {

        float alpha = i * M_PI / 10;    // This is the angle cause there are 10 triangles per pi
        float Tx = i * 0.013;
        float Ty = 0;
        float scale = i * 0.0015;   // We don't do a first scale even if this is what you probably did
                                    // for the example because it's a bad use of resources for us

        Transform _M = Transform::Identity();
        _M *= Eigen::AngleAxisf(alpha, Eigen::Vector3f::UnitZ());
        _M *= Eigen::Translation3f(Tx,Ty,0);
        _M *= Eigen::AlignedScaling3f(scale, scale, scale);

        mat4 M = _M.matrix();

        triangle.draw(M);
    }

}

void fermat () {
    glClear(GL_COLOR_BUFFER_BIT);

    // We want a reduced triangle first
    float scale = 0.02;

#if 0

    /*     --- HARD MODE ! ---
     * That's a point per point calculation !
     */

    mat4 N = mat4::Identity();
    N(0,0) = scale;
    N(1,1) = scale;


    for(int i = 0; i < 600; i++) {
        float alpha = i * 137.508f * M_PI/360.0f;
        float r = (2 * scale) * sqrt(alpha);
        N(0,3) = r * cos(alpha);
        N(1,3) = r * sin(alpha);

        triangle.draw(N);
    }
#endif

    // With tranfos mode
    for (int i = 0; i < 500; i++)
    {
        // See Wikipedia or slide 8 for those things
        float alpha = i * 137.508f * M_PI/360.0f;
        float r = (2*scale) * sqrt(alpha);  // We take a constant c = 2*scale
        // As we rotate first we can translate just r, the *sin(alpha) and *cos(alpha) will be done automatically
        float Tx = r * cos(alpha);
        float Ty = r * sin(alpha);

        Transform _M = Transform::Identity();

        _M *= Eigen::Translation3f(Tx,Ty,0);
        _M *= Eigen::AlignedScaling3f(scale, scale, scale);
        _M *= Eigen::AngleAxisf(alpha, Eigen::Vector3f::UnitZ());

        mat4 M = _M.matrix();

        triangle.draw(M);
    }


}


void init(){
    glClearColor(/*gray*/ .937,.937,.937, /*solid*/1.0 );
    triangle.init();
}

int main(int, char**){
    glfwInitWindowSize(512, 512);

#if USE_SPIRAL
    glfwCreateWindow("spiral");
    glfwDisplayFunc(spiral);
#else
    glfwCreateWindow("fermat");
    glfwDisplayFunc(fermat);
#endif
    init();
    glfwMainLoop();
    triangle.cleanup();
    return EXIT_SUCCESS;
}


