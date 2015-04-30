#version 400

uniform mat4 depth_vp;
uniform mat4 model;

in vec3 vpoint;

void main() {
    gl_Position = depth_vp * model* vec4(vpoint,1);
}
