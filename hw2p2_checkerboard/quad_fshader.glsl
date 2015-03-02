#version 330 core
uniform sampler1D colormap;
in vec2 uv;
out vec3 color;

void main()
{
    float value = ((sin(uv.x * 31.5) * sin(uv.y * 31.5)) + 1) / 2;
    color = texture(colormap, value).rgb;

}
