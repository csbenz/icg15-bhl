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
out vec3 light_dir;
out vec3 view_dir;

void main() {
    mat4 MV = view * model;
    vec4 vpoint_mv = MV * vec4(vpoint, 1.0);
    gl_Position = projection * vpoint_mv;

    vcolor = vec3(0.0,0.0,0.0);
    vec3 normal_mv = mat3(transpose(inverse(MV))) * vnormal;
    light_dir = light_pos - vpoint_mv.xyz;
    view_dir = -vpoint_mv.xyz;

    vcolor += Ia*ka;
    vec3 N = normalize(normal_mv);
    vec3 L = normalize(light_dir);
    float lambert = dot(N,L);
    if(lambert > 0.0) {
        vcolor += Id*kd*lambert;
        vec3 V = normalize(view_dir);
        vec3 R = reflect(-L,N);
        vcolor += Is*ks*pow(max(dot(R,V), 0.0), p);
    }
}
