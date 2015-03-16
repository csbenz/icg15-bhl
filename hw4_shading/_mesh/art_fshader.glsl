// Copyright (C) 2014 - LGG EPFL
#version 330 core
uniform vec3 Ia, Id, Is;
uniform vec3 ka, kd, ks;
uniform float p;

uniform sampler2D tex2D;

in vec4 normal_mv;
in vec4 light_dir;
in vec4 view_dir;

out vec3 color;

void main() {
    ///>>>>>>>>>> TODO >>>>>>>>>>>
    /// TODO 3.2: Artistic shading.
    /// 1) compute the output color by doing a look-up in the texture using the texture sampler tex.
    ///<<<<<<<<<< TODO <<<<<<<<<<<
    vec4 R = normalize(2.0f * dot(light_dir, normal_mv) * normal_mv - light_dir);

    float VR = dot(view_dir, R);
    if (VR < 0.0)
        VR = 0.0;
    float NL = dot(normal_mv, light_dir);
    if (NL < 0.0)
        NL = 0.0;

    color = texture2D(tex2D, vec2(NL, pow(VR, p)));
}

