#version 330 core
uniform sampler2D tex;
uniform float tex_width;
uniform float tex_height;
in vec2 uv;
out vec3 color;

float rgb_2_luma(vec3 c){ return .3*c[0] + .59*c[1] + .11*c[2]; }

void main() {
    color = texture(tex,uv).rgb;
    /// TODO1: edge detection goes here
    /// TODO2: gaussian convolution goes here
}
