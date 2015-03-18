// Copyright (C) 2014 - LGG EPFL
#version 330 core
uniform vec3 Ia, Id, Is;
uniform vec3 ka, kd, ks;
uniform float p;

uniform sampler1D tex1D;

in vec3 normal_mv;
in vec3 light_dir;
in vec3 view_dir;

out vec3 color;

void main() {
    color = vec3(0.0,0.0,0.0);

    ///>>>>>>>>>> TODO >>>>>>>>>>>
    /// TODO 2.2: Toon shading.
    /// 1) compute ambient term.
    /// 2) compute diffuse term using the texture sampler tex.
    /// 3) compute specular term using the texture sampler tex.
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

    vec3 ambient = Ia * ka;
    vec3 diffuse = Id * kd * texture(tex1D, NL).r;
    vec3 specular = Is * ks * texture(tex1D, pow(RV, p)).r;

    color = ambient + diffuse + specular;
}
