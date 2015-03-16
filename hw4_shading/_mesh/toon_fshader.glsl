// Copyright (C) 2014 - LGG EPFL
#version 330 core

uniform sampler1D tex1D;
uniform vec3 Ia, Id, Is;
uniform vec3 ka, kd, ks;
uniform float p;

in vec4 normal_mv;
in vec4 light_dir;
in vec4 view_dir;

out vec3 color;

void main() {

    ///>>>>>>>>>> TODO >>>>>>>>>>>
    /// TODO 2.2: Toon shading.
    /// 1) compute ambient term.

    /// 3) compute specular term using the texture sampler tex.
    ///<<<<<<<<<< TODO <<<<<<<<<<<
    vec4 R = normalize(2.0f * dot(light_dir, normal_mv) * normal_mv - light_dir);

    float VR = dot(view_dir, R);
    if (VR < 0.0)
        VR = 0.0;
    float NL = dot(normal_mv, light_dir);
    if (NL < 0.0)
        NL = 0.0;

    vec3 diffuse = Id * kd * texture1D(tex1D, NL).r;
    vec3 specular = Is * ks * texture1D(tex1D, pow(VR, p)).r;

    color = Ia * ka + diffuse + specular;
}
