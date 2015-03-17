// Copyright (C) 2014 - LGG EPFL
#version 330 core

uniform vec3 Ia, Id, Is;
uniform vec3 ka, kd, ks;
uniform float p;

uniform sampler2D tex2D;

in vec3 normal_mv;
in vec3 light_dir;
in vec3 view_dir;

out vec3 color;

void main() {

    ///>>>>>>>>>> TODO >>>>>>>>>>>
    /// TODO 3.2: Artistic shading.
    /// 1) compute the output color by doing a look-up in the texture using the texture sampler tex.
    ///<<<<<<<<<< TODO <<<<<<<<<<<

    vec3 R = normalize(2.0 * normal_mv * (dot(normal_mv, light_dir)) - light_dir);
    //R = normalize(reflect(-light_dir, normal_mv)); // alternative, simpler

    float RV = dot(R, view_dir);
    float NL = dot(normal_mv, light_dir);

    if(RV < 0.0) {
        RV = 0.0;
    }

    if(NL < 0.0) {
        NL = 0.0;
    }

    color = texture(tex2D, vec2(NL, pow(RV, p))).rgb;
}
