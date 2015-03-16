#version 330 core 
uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform vec3 light_pos;
uniform vec3 spot_dir;

in vec3 vpoint;
in vec3 vnormal;

out vec4 normal_mv;
out vec4 light_dir;
out vec4 view_dir;
out vec4 spot_direction;

void main() {
    mat4 MV = view * model;
    vec4 vpoint_mv = MV * vec4(vpoint, 1.0);
    gl_Position = projection * vpoint_mv;

    normal_mv = normalize(MV * vec4(vnormal, 0.0f));

    light_dir = normalize(vec4(light_pos, 1.0f) - vpoint_mv);

    view_dir = normalize(vec4(0.0, 0.0, 0.0, 1.0f) - vpoint_mv);
    spot_direction = normalize(vec4(spot_dir, 0.0f));
}

