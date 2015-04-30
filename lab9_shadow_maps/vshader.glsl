#version 330 core
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 depth_vp_offset;
in vec3 vpoint;
in vec3 vnormal;
in vec2 vtexcoord;
out vec3 n;
out vec2 uv;
out vec4 shadow_coord;

void main() {
    n = mat3(model) * vnormal;

    shadow_coord = depth_vp_offset * model * vec4(vpoint,1.0);


    /* TODO: Transform and projection the point into the shadow map, xy should
        be uv coordinates and z the depth value you want to compare to */

    gl_Position = projection * view * model * vec4(vpoint, 1.0);
    uv = vtexcoord;
}
