#version 330 core
in vec3 vpoint;
uniform mat4 M;

void main() {
    gl_Position = M * vec4(vpoint, 1.0);
}
