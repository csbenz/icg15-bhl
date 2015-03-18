#version 330 core
uniform vec3 Ia, Id, Is;
uniform vec3 ka, kd, ks;
uniform float p;

const float spot_cos_cutoff = 0.985; // cos 10 degrees
const float spot_exp = 150;

in vec3 normal_mv;
in vec3 light_dir;
in vec3 view_dir;
in vec3 spot_dir_mv;

out vec3 color;

void main() {    

    ///>>>>>>>>>> TODO >>>>>>>>>>>
    /// TODO 5: Spot light.
    /// Complete the shader to obtain spot light effect
    ///<<<<<<<<<< TODO <<<<<<<<<<<

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

    vec3 I = (Ia*ka) + (Id*kd)*NL + (Is*ks)*pow(RV, p);

    if (dot(light_dir, spot_dir_mv) > spot_cos_cutoff) {
       float spotEff = pow(dot(light_dir, spot_dir_mv), spot_exp);
       color = I * spotEff;
    } else {
       color = vec3(0.0f);
    }
}
