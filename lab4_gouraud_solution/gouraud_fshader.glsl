// Copyright (C) 2014 - LGG EPFL
#version 330 core

in vec3 vcolor;
in vec3 light_dir;
in vec3 view_dir;
out vec3 color;

void main() {
    color = vcolor;
}
