// Copyright (C) 2014 - LGG EPFL
#version 330 core 
uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform vec3 light_pos;

in vec3 vpoint;
in vec3 vnormal;

out vec4 normal_mv;
out vec4 light_dir;
out vec4 view_dir;

void main() {
    mat4 MV = view * model;
    vec4 vpoint_mv = MV * vec4(vpoint, 1.0);
    gl_Position = projection * vpoint_mv;

    /// TODO 1.1: Phong shading.
    /// 1) compute normal_mv using the model_view matrix.
    normal_mv = normalize(MV * vec4(vnormal, 0.0f));
    /// 2) compute the light direction light_dir.
    light_dir = normalize(vec4(light_pos, 1.0f) - vpoint_mv);
    /// 3) compute the view direction view_dir.
    view_dir = normalize(vec4(0.0, 0.0, 0.0, 1.0f) - vpoint_mv);
}

