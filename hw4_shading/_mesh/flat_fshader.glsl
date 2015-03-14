// Copyright (C) 2014 - LGG EPFL
#version 330 core
uniform vec3 Ia, Id, Is;
uniform vec3 ka, kd, ks;
uniform float p;

in vec4 vpoint_mv;
in vec3 light_dir, view_dir;

out vec3 color;

void main() {

    vec3 derX = dFdx(vpoint_mv.xyz);
    vec3 derY = dFdy(vpoint_mv.xyz);
    vec3 normal_mv = normalize(cross(derX, derY));

    vec3 R = normalize(2.0f * dot(light_dir, normal_mv) * normal_mv - light_dir);

    float VR = dot(view_dir, R);
    if (VR < 0.0) {
        VR = 0.0;
    }

    float NL = dot(normal_mv, light_dir);
    if (NL < 0.0){
        NL = 0.0;
    }

    color = (Ia*ka) + (Id*kd)*NL + (Is*ks)*pow(VR, p);
}

