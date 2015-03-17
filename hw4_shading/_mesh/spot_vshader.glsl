#version 330 core 
uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform vec3 light_pos;
uniform vec3 spot_dir;

in vec3 vpoint;
in vec3 vnormal;

out vec3 normal_mv;
out vec3 light_dir;
out vec3 view_dir;
out vec3 spot_dir_mv;

void main() {
    mat4 MV = view * model;
    vec4 vpoint_mv = MV * vec4(vpoint, 1.0);
    gl_Position = projection * vpoint_mv;

    normal_mv = normalize((inverse(transpose(MV)) * vec4(vnormal, 0.0)).xyz);
    light_dir = normalize(light_pos - vpoint_mv.xyz);
    view_dir = normalize(-vpoint_mv.xyz);
    spot_dir_mv = normalize(spot_dir);
}
