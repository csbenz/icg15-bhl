#version 330 core
in vec2 uv;
uniform sampler2D colorTex;
uniform sampler2D velocityTex;
out vec4 color;

void main() {
    /// TODO: use the velocity vector stored in velocityTex to compute the line integral
    /// TODO: use a constant number of samples for integral (what happens if you take too few?)
    /// HINT: you can scale the velocity vector to make the motion blur effect more prominent
    /// HINT: to debug integration don't use the velocityTex, simply assume velocity is constant, e.g. vec2(1,0)

    const int n_samples = 100;
    const float scale = 1.0f;


    vec2 velocity = texture(velocityTex, uv).xy * scale;

    vec4 tot_color = vec4(0);

    for (int i = 1; i <= n_samples; i++) {
        tot_color += texture(colorTex, uv + float(i)/n_samples * velocity);

    }

    color = tot_color / n_samples;

}

