#version 400

uniform mat4 depth_vp;
uniform mat4 model;

in vec3 vpoint;

void main() {
    gl_Position = /* TODO: Transform and projection the point into light's NDC */ vec4(vpoint, 1.0);
}
