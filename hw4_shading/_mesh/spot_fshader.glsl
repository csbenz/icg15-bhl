#version 330 core
uniform vec3 Ia, Id, Is;
uniform vec3 ka, kd, ks;
uniform float p;

const float spot_cos_cutoff = 0.985; // cos 10 degrees
const float spot_exp = 150;

in vec4 normal_mv;
in vec4 light_dir;
in vec4 view_dir;
in vec4 spot_direction;

out vec3 color;

void main() {    

    vec4 R = normalize(2.0f * dot(light_dir, normal_mv) * normal_mv - light_dir);

    float VR = dot(view_dir, R);
    if (VR < 0.0)
        VR = 0.0;
    float NL = dot(normal_mv, light_dir);
    if (NL < 0.0)
        NL = 0.0;

    vec3 I = (Ia*ka) + (Id*kd)*NL + (Is*ks)*pow(VR, p);

    ///>>>>>>>>>> TODO >>>>>>>>>>>
    /// TODO 5: Spot light.
    /// Complete the shader to obtain spot light effect


    if (dot(light_dir, spot_direction) > spot_cos_cutoff) {
        float spotEff = pow(dot(light_dir, spot_direction), spot_exp);
        color = I * spotEff;
    }
    else
    {
        color = vec3(0.0f);
    }
}

