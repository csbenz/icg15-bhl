#include "icg_common.h"
#include "Quad.h"

Quad sun;
Quad earth;
Quad moon;

void init()
{
	glClearColor(/*white*/ 1.0f,1.0f,1.0f, /*solid*/1.0 );

	/// {stuff}.init(...);
	sun.init("sun.tga");
	earth.init("earth.tga");
	moon.init("moon.tga");
}

void drawSun(Quad* sun, const float time)
{
	/* move sun a bit on the right */
	mat4 translation = mat4::Identity();
	translation(0,3) = 0.2; /* x-axis translation */
	translation(1,3) = 0; /* y-axis translation */

	/* rotation on itself */
	const float sunRotationSpeed = 0.3;
	mat4 rotation = mat4::Identity();
	rotation(0,0) = cosf(sunRotationSpeed * time);
	rotation(0,1) = -sinf(sunRotationSpeed * time);
	rotation(1,0) = sinf(sunRotationSpeed * time);
	rotation(1,1) = cosf(sunRotationSpeed * time);

	/* automatic scaling of the size of earth */
	const float basicScale = 0.2;
	const float scaleSpeed = 0.5;
	const float scaleVariance = 0.1;
	mat4 scale = mat4::Identity();
	/*
	 * I do use a special function here for scaling,
	 * - basicScale is setting an average scale
	 * - scaleSpeed * time is setting a slowly scaling speed througout time
	 * - cos(X) + 1 will move the scale around 1 (so near basicScale)
	 * - then I minimize the effect of the cosine with scaleVariance
	 */
	scale(0,0) = basicScale * ((scaleVariance * cosf(scaleSpeed * time)) + 1);
	scale(1,1) = basicScale * ((scaleVariance * cosf(scaleSpeed * time)) + 1);;

	const mat4 M = translation * rotation * scale;

	sun->draw(M);
}

void drawEarth(Quad* earth, const float time)
{
	/* rotation of the earth on itself */
	const float rotationSpeedCenter = 1.2;
	mat4 rotationCenter = mat4::Identity();
	rotationCenter(0,0) = cosf(rotationSpeedCenter * time);
	rotationCenter(0,1) = -sinf(rotationSpeedCenter * time);
	rotationCenter(1,0) = sinf(rotationSpeedCenter * time);
	rotationCenter(1,1) = cosf(rotationSpeedCenter * time);

	/* translation to distance from sun to earth */
	mat4 translation = mat4::Identity();
	/* same trick used here as I applied for the sun scale */
	translation(0,3) = 0.7 * ((0.2 * cosf(time)) + 1); /* x-axis translation */
	translation(1,3) = 0; /* y-axis translation */

	/* rotation around sun */
	const float rotationSpeedSun = -0.5;
	mat4 rotation = mat4::Identity();
	rotation(0,0) = cosf(rotationSpeedSun * time);
	rotation(0,1) = -sinf(rotationSpeedSun * time);
	rotation(1,0) = sinf(rotationSpeedSun * time);
	rotation(1,1) = cosf(rotationSpeedSun * time);

	/* basic earth scale */
	mat4 scale = mat4::Identity();
	scale(0,0) = 0.1;
	scale(1,1) = 0.1;

	const mat4 M = rotation * translation * rotationCenter * scale;

	earth->draw(M);
}

void drawMoon(Quad* moon, const float time)
{
	/* rotation of moon on itself */
	mat4 rotationCenter = mat4::Identity();
	rotationCenter(0,0) = cosf(time);
	rotationCenter(0,1) = -sinf(time);
	rotationCenter(1,0) = sinf(time);
	rotationCenter(1,1) = cosf(time);

	/* radius from earth */
	mat4 translationEarth = mat4::Identity();
	translationEarth(0,3) = 0.2; /* x-axis translation */
	translationEarth(1,3) = 0; /* y-axis translation */

	/* rotation around earth */
	const float rotationSpeedEarth = 1.6;
	mat4 rotationEarth = mat4::Identity();
	rotationEarth(0,0) = cosf(rotationSpeedEarth * time);
	rotationEarth(0,1) = -sinf(rotationSpeedEarth * time);
	rotationEarth(1,0) = sinf(rotationSpeedEarth * time);
	rotationEarth(1,1) = cosf(rotationSpeedEarth * time);

	/* radius around the sun - should be similar to earth's */
	mat4 translationSun = mat4::Identity();
	translationSun(0,3) = 0.7 * ((0.2 * cosf(time)) + 1); /* x-axis translation */
	translationSun(1,3) = 0; /* y-axis translation */

	/* rotation around the sun - should be similar to earth's */
	const float rotationSpeedSun = -0.5;
	mat4 rotationSun = mat4::Identity();
	rotationSun(0,0) = cosf(rotationSpeedSun * time);
	rotationSun(0,1) = -sinf(rotationSpeedSun * time);
	rotationSun(1,0) = sinf(rotationSpeedSun * time);
	rotationSun(1,1) = cosf(rotationSpeedSun * time);

	mat4 scale = mat4::Identity();
	/* values experimentally found */
	scale(0,0) = 0.03;
	scale(1,1) = 0.03;

	const mat4 M = rotationSun * translationSun * rotationEarth *
		translationEarth * rotationCenter *  scale;

	moon->draw(M);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	float time_s = glfwGetTime();

	/// {stuff}.draw(...);
	drawSun(&sun, time_s);
	drawEarth(&earth, time_s);
	drawMoon(&moon, time_s);
}

int main(int, char**)
{
	glfwInitWindowSize(800, 800);
	glfwCreateWindow("planets");
	glfwDisplayFunc(display);
	init();
	glfwMainLoop();

	//// {stuff}.cleanup()
	sun.cleanup();
	earth.cleanup();
	moon.cleanup();
	return EXIT_SUCCESS;
}
