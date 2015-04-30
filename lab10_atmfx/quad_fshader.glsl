#version 330
uniform float camera_height;            // Camera height from ground
uniform float I_sun;                    // Sunlight irradiance (assume white light)
uniform float H_0;                      // Height of average density [0,1]
uniform vec3 K_ray;                     // Scattering constant (Rayleigh)
uniform float K_mie;                    // Scattering constant (Mie)
uniform int nb_samples;                 // Number of integration step
uniform float inner_radius;             // Distance from the center to the surface of the planet
uniform float outer_radius;             // Distance from the center to the end of the atmosphere
uniform vec3 light_dir;                 // Light direction (world space)
uniform float window_width;             // Used for ray direction computation
uniform float window_height;            // Used for ray direction computation
uniform mat4 camera_matrix_inverse;     // Used for ray direction computation
out vec4 frag_color;                    // Final fragment color (shaded sky pixel)

const float M_PI=3.1415926535897932384626433832795;

/// Checks whether ray intersects a sphere
bool is_intersection (vec3 p, vec3 ray, float radius){
    float height2 = dot (p, p);
    //float A = dot (ray, ray) = 1 beacause ray is normalized
    float B = 2.0 * dot (p, ray);
    float C = height2 - radius * radius;
    float det = (B * B - 4.0 * C);
    return (B < 0.0 && det >= 0.0 && -B > sqrt (det));
}

/// Returns (ray-parameterized) intersection point with sphere.
//  The line is parameterized as p = ray.origin + t*ray.dir. You are returning two values 
//  for "t" corresponding to the two solutions (near, far) of a line/sphere intersection.
vec2 line_circle_intersect (vec3 p, vec3 ray, float radius){
    float height2 = dot (p, p);
    //float A = dot (ray, ray) = 1 beacause ray is normalized
    float B = 2.0 * dot (p, ray);
    float C = height2 - radius * radius;
    float det = max (0.0, B * B - 4.0 * C);
    float near = max (0.0, 0.5 * (-B - sqrt (det)));
    float far = 0.5 * (-B + sqrt (det));
    return vec2 (near, far);
}

/// Phase function - Mie scattering
float F_mie(float theta){
    return 0.0;
}

/// Phase function - Rayleigh scattering
float F_ray(float theta){ 
    return 0.0;
}


void main (){
    ///--- Camera position
    vec3 cam_pos = vec3(0.0, inner_radius + camera_height, 0.0);
    
    ///--- Compute ray direction (one ray per pixel)
    vec3 ray = vec3(0,0,-1.0);
    ray.x = (gl_FragCoord.x / window_width - 0.5) * 2.0;
    ray.y = (gl_FragCoord.y / window_height - 0.5) * 2.0;
    // Conserve aspect ratio and use a fovy of 60⁰ (30⁰ = 0.52rad)
    // Ask TA "Luc Girod" why this is done this way if you care.
    ray.x *= sin(0.52) * (window_width/window_height); 
    ray.y *= sin(0.52);         
    ray = normalize(ray);
    ray = mat3(camera_matrix_inverse) * ray;
    
    ///--- Check if we interset the inner sphere
    if(is_intersection (cam_pos, ray, inner_radius)){
        frag_color = vec4 (0.5, 0.8, 0.5, 1.0);
        return;
    }
    
    ///--- TASK: evaluate the in-scattering integral
    frag_color = vec4 (0.0,0.0,1.0, 1.0);
}

