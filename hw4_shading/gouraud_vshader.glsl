// Copyright (C) 2014 - LGG EPFL
#version 330 core 
uniform vec3 Ia, Id, Is;
uniform vec3 ka, kd, ks;
uniform float p;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform vec3 light_pos; // en pos camera

in vec3 vpoint;
in vec3 vnormal;

out vec3 vcolor;

void main() {
    mat4 MV = view * model;
    vec3 vpoint_mv = (MV * vec4(vpoint, 1.0)).xyz;
    gl_Position = projection * vec4(vpoint_mv, 1.0);

    ///>>>>>>>>>> TODO >>>>>>>>>>>
    /// TODO 0.1: Gouraud shading.
    /// 1) compute the normal using the model_view matrix.
    /// 2) compute the light direction light_dir.
    /// 3) compute the view direction view_dir.
    /// 4) compute per vertex color
    ///<<<<<<<<<< TODO <<<<<<<<<<<

    vec3 normal_mv = normalize((inverse(transpose(MV)) * vec4(vnormal, 0.0)).xyz);
    vec3 light_dir = normalize(light_pos - vpoint_mv);
    vec3 view_dir = normalize(-vpoint_mv);

    // formula : R = 2N(N · L) - L
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

    vcolor = (Ia*ka) + (Id*kd)*NL + (Is*ks)*pow(RV, p);
}
