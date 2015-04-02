#version 330 core
uniform sampler2D tex;
uniform sampler2D temp_tex;
uniform float tex_width;
uniform float tex_height;
uniform int times;
uniform float[100] kernel;
uniform int SIZE;
uniform int mode;
uniform float std;
in vec2 uv;
layout (location = 0) out vec3 color;

float rgb_2_luma(vec3 c){ return .3*c[0] + .59*c[1] + .11*c[2]; }

void main() {
#if 0
    ///--- x=-1
    float t_00 = rgb_2_luma( textureOffset(tex, uv, ivec2(-1, -1)).rgb );
    float t_01 = rgb_2_luma( textureOffset(tex, uv, ivec2(-1,  0)).rgb );
    float t_02 = rgb_2_luma( textureOffset(tex, uv, ivec2(-1, +1)).rgb );
    ///--- x=0
    float t_10 = rgb_2_luma( textureOffset(tex, uv, ivec2( 0, -1)).rgb );
    float t_12 = rgb_2_luma( textureOffset(tex, uv, ivec2( 0, +1)).rgb );
    ///--- x=+1
    float t_20 = rgb_2_luma( textureOffset(tex, uv, ivec2(+1, -1)).rgb );
    float t_21 = rgb_2_luma( textureOffset(tex, uv, ivec2(+1,  0)).rgb );
    float t_22 = rgb_2_luma( textureOffset(tex, uv, ivec2(+1, +1)).rgb );
    ///--- Gradients
    float sx = -1*t_00 -2*t_01 -1*t_02    +1*t_20 +2*t_21 +1*t_22;
    float sy = -1*t_00 -2*t_10 -1*t_20    +1*t_02 +2*t_12 +1*t_22;
    float g = 1-sqrt(sx*sx + sy*sy);

    color = vec3(g, g, g);
#else
    ///--- Gaussian convolution
    if(mode == 0){
        if(times == 1){ // So we will do it only once
            vec3 color_tot = vec3(0);
            float weight_tot = 0;
            for(int i=-SIZE; i<=SIZE; i++){
                for(int j=-SIZE; j<=SIZE; j++){
                    float w = exp(-(i*i+j*j)/(2.0*std*std*std*std));
                    vec3 neigh_color = texture(tex, uv+vec2(i/tex_width,j/tex_height)).rgb;
                    color_tot += w * neigh_color;
                    weight_tot += w;
                }
            }
            color = color_tot/weight_tot;
        }
    }
    else{
        
        vec3 gauss_color = vec3(0);
        if(times == 0){
            for(int i=-SIZE; i<=SIZE; i++){
                gauss_color += texture(tex, uv+vec2(i/tex_width,0)).rgb * kernel[i+SIZE];
            }
        }
        else{
            for(int i=-SIZE; i<=SIZE; i++){
                gauss_color += texture(temp_tex, uv+vec2(0,i/tex_height)).rgb * kernel[i+SIZE];
            }
        }
        color = gauss_color;
    }
#endif
}

