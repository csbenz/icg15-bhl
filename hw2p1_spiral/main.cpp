#include "icg_common.h"
#include "Triangle.h"

#define USE_SPIRAL 0

Triangle triangle;

typedef Eigen::Transform<float, 3, Eigen::Affine> Transform;

void spiral()
{
	glClear(GL_COLOR_BUFFER_BIT);
	int const numTriangles = 60;

	for (int i = 0; i < numTriangles; ++i) {
		float const alpha = i * M_PI / 10; /* this is the angle cause there are 10 triangles per pi */
		float const tx = i * 0.013;
		float const ty = 0;
		/* we don't do a first scale even if this is what you probably did */
		/* for the example because it's a bad use of resources for us */
		float const scale = i * 0.0015;

		Transform _M = Transform::Identity();
		_M *= Eigen::AngleAxisf(alpha, Eigen::Vector3f::UnitZ());
		_M *= Eigen::Translation3f(tx, ty, 0);
		_M *= Eigen::AlignedScaling3f(scale, scale, scale);

		mat4 M = _M.matrix();

		triangle.draw(M);
	}
}

void fermat()
{
	glClear(GL_COLOR_BUFFER_BIT);
	float const goldenNumber = 137.508;

	/* we want a reduced triangle first */
	float const scale = 0.02;

#if 0
	/*
	 * That's a point per point calculation !
	 */
	int const numTriangles = 600;

	mat4 N = mat4::Identity();
	N(0,0) = scale;
	N(1,1) = scale;

	for(int i = 0; i < numTriangles; ++i) {
		float const alpha = i * goldenNumber * (M_PI / 360.0);
		float const r = (2 * scale) * sqrt(alpha);
		N(0,3) = r * cos(alpha);
		N(1,3) = r * sin(alpha);

		triangle.draw(N);
	}
#else
	int const numTriangles = 500;
	/* with tranfos mode */
	for (int i = 0; i < numTriangles; ++i) {
		/* see Wikipedia or slide 8 for those things */
		float const alpha = i * goldenNumber * (M_PI / 360.0);
		/* we take a constant c = 2 * scale */
		float const r = (2 * scale) * sqrt(alpha);
		/* as we rotate first we can translate just r, the *sin(alpha) and *cos(alpha) will be done automatically */
		float const tx = r * cos(alpha);
		float const ty = r * sin(alpha);

		Transform _M = Transform::Identity();

		_M *= Eigen::Translation3f(tx, ty, 0);
		_M *= Eigen::AlignedScaling3f(scale, scale, scale);
		_M *= Eigen::AngleAxisf(alpha, Eigen::Vector3f::UnitZ());

		mat4 M = _M.matrix();

		triangle.draw(M);
	}
#endif
}


void init()
{
	glClearColor(/*gray*/ .937, .937, .937, /*solid*/ 1.0);
	triangle.init();
}

int main(int, char**)
{
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
