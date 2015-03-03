#version 330 core
uniform sampler1D colormap;
in vec2 uv;
out vec3 color;

#define M_PI 3.14159265358979323846

void main()
{
    float value = ((sin(uv.x * 10 * M_PI) * sin(uv.y * 10 * M_PI)) + 1) / 2;
    color = texture(colormap, value).rgb;

}
