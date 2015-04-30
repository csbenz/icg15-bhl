#version 330 core

in vec3 n;  // incoming normal;

out vec3 color;

uniform bool use_color;  // Use predefined color or texture?

// For texturing
uniform sampler2D tex;
in vec2 uv;

// For coloring
uniform vec3 mesh_color;


uniform vec3 light_dir;
uniform float bias;

uniform sampler2D shadow_map;
in vec4 shadow_coord;

// Poisson disk sample locations.
vec2 poisson_disk[16] = vec2[](
   vec2(-0.94201624, -0.39906216),
   vec2(0.94558609, -0.76890725),
   vec2(-0.094184101, -0.92938870),
   vec2(0.34495938, 0.29387760),
   vec2(-0.91588581, 0.45771432),
   vec2(-0.81544232, -0.87912464),
   vec2(-0.38277543, 0.27676845),
   vec2(0.97484398, 0.75648379),
   vec2(0.44323325, -0.97511554),
   vec2(0.53742981, -0.47373420),
   vec2(-0.26496911, -0.41893023),
   vec2(0.79197514, 0.19090188),
   vec2(-0.24188840, 0.99706507),
   vec2(-0.81409955, 0.91437590),
   vec2(0.19984126, 0.78641367),
   vec2(0.14383161, -0.14100790)
);

void main() {
    float ambient_light = 0.0;
    float shade = ambient_light + max(dot(normalize(n), normalize(light_dir)), 0.0);

    float shadow = 1.0;  // shading factor from the shadow (1.0 = no shadow, 0.0 = all dark)
    vec2 dimension = textureSize(shadow_map,0);

    float col = texture(shadow_map, shadow_coord.xy).r;
    if (col < shadow_coord.z-bias){
        shadow = 0.2;
    }
    // TODO: Shadow Mapping
    // 1. Do perspective division of 'shadow_coord'.
    // 2. Get the sample of the shadow map at 'shadow_coord.xy'.
    float spread = 200;
    for (int i = 0; i < 16; i++) {
        if (texture(shadow_map, shadow_coord.xy + poisson_disk[i]/spread).r < shadow_coord.z - bias) {
            shadow -= (1.0/20.0);
        }
    }

    // 3. Compare the sample with the distance of the current pixel to the light (stored in 'shadow_coord.z').
    // 4. Apply biasing by shortening the distance a little bit (you can use the uniform 'bias').
    // 5. You can to PCF if you want to (16 samples within a a circle are stored in 'poisson_disk', you can use those'),
    //    Experiment with different radii for the sampling circles around 'shadow_coord.xy'.


    if (use_color) {
        color = shadow * shade * mesh_color;
    } else {
        color = shadow * shade * texture(tex, uv).rgb;
    }
    //color = vec3(texture(shadow_map,uv).r);
}
