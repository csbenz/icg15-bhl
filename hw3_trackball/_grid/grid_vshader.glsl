#version 330 core

uniform mat4 mvp;

uniform float time;

in vec2 position;
out vec2 uv;



void main() {
    float M_PI = 3.14159265358979323846;
    const float WAVES = 3;
    const float WAVE_HEIGHT = 1.0f / 15.0f;

    uv = (position + vec2(1.0, 1.0)) * 0.5;

    // Convert the 2D position into 3D positions that all lay in a horizontal
    // plane.
    // TODO 6: Animate the height of the grid points as a sine function of the
    // 'time' and the position ('uv') within the grid.
    float height = sin(time + (WAVES * M_PI * (uv.x + uv.y))) * WAVE_HEIGHT;
    vec3 pos_3d = vec3(position.x, height, -position.y);

    gl_Position = mvp * vec4(pos_3d, 1.0);
}
