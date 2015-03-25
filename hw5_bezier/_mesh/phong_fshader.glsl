// Copyright (C) 2014 - LGG EPFL
#version 330 core
uniform vec3 Ia, Id, Is;
uniform vec3 ka, kd, ks;
uniform float p;

in vec3 normal_mv;
in vec3 light_dir;
in vec3 view_dir;

out vec3 color;

void main() {

        vec3 R = normalize(reflect(-light_dir, normal_mv));

        float RV = dot(R, view_dir);
        float NL = dot(normal_mv, light_dir);

        if(RV < 0.0) {
            RV = 0.0;
        }

        if(NL < 0.0) {
            NL = 0.0;
        }

        color = (Ia*ka) + (Id*kd)*NL + (Is*ks)*pow(RV, p);
}
