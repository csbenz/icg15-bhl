// Copyright (C) 2014 - LGG EPFL
#version 330 core 
uniform vec3 Ia, Id, Is;
uniform vec3 ka, kd, ks;
uniform float p;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform vec3 light_pos;

in vec3 vpoint;
in vec3 vnormal;

out vec3 vcolor;


void main() {
    mat4 MV = view * model;
    vec4 vpoint_mv = MV * vec4(vpoint, 1.0);
    gl_Position = projection * vpoint_mv;
    ///>>>>>>>>>> TODO >>>>>>>>>>>
    /// TODO 0.1: Gouraud shading.
    /// 1) compute the normal using the model_view matrix.
    vec4 normal_mv = normalize(MV * vec4(vnormal, 0.0f));
    /// 2) compute the light direction light_dir.
    vec4 light_dir = normalize(vec4(light_pos, 1.0f) - vpoint_mv);
    /// 3) compute the view direction view_dir.
    vec4 view_dir = normalize(vec4(0.0, 0.0, 0.0, 1.0f) - vpoint_mv);
    /// 4) compute per vertex color

    vec4 R = normalize(2.0f * dot(light_dir, normal_mv) * normal_mv - light_dir);

    float VR = dot(view_dir, R);
    if (VR < 0.0)
        VR = 0.0;
    float NL = dot(normal_mv, light_dir);
    if (NL < 0.0)
        NL = 0.0;

    vcolor = (Ia*ka) + (Id*kd)*NL + (Is*ks)*pow(VR, p);
}
