// Copyright (C) 2014 - LGG EPFL
#version 330 core 
uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform vec3 light_pos;

in vec3 vpoint;
in vec3 vnormal;

out vec3 normal_mv;
out vec3 light_dir;
out vec3 view_dir;

void main() {
    mat4 MV = view * model;
    vec4 vpoint_mv = MV * vec4(vpoint, 1.0f);
    gl_Position = projection * vpoint_mv;
    normal_mv = normalize((inverse(transpose(MV)) * vec4(vnormal, 0.0f)).xyz);
    light_dir = normalize(light_pos - vpoint_mv.xyz);
    view_dir = normalize(-vpoint_mv.xyz);
}
