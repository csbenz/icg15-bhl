#version 330 core

#define EFFECT_TYPE 2

uniform sampler2D tex;
uniform float tex_width;
uniform float tex_height;
in vec2 uv;
out vec3 color;
uniform vec2 direction;
uniform float weights[11];

float rgb_2_luma(vec3 c)
{
	return 0.3 * c.x + 0.59 * c.y + 0.11 * c.z;
}

float gauss(float x)
{
	float sigma = 2.0;
	return pow(2.718, - ((x * x) / (2.0 * (sigma * sigma))));
}

void main()
{
#if EFFECT_TYPE == 0
	/// TODO1: edge detection goes here
	float i00 = rgb_2_luma(textureOffset(tex, uv, ivec2(-1, -1)).rgb);
	float i01 = rgb_2_luma(textureOffset(tex, uv, ivec2(-1, 0)).rgb);
	float i02 = rgb_2_luma(textureOffset(tex, uv, ivec2(-1, 1)).rgb);

	float i10 = rgb_2_luma(textureOffset(tex, uv, ivec2(0, -1)).rgb);
	float i11 = rgb_2_luma(textureOffset(tex, uv, ivec2(0, 0)).rgb);
	float i12 = rgb_2_luma(textureOffset(tex, uv, ivec2(0, 1)).rgb);

	float i20 = rgb_2_luma(textureOffset(tex, uv, ivec2(1, -1)).rgb);
	float i21 = rgb_2_luma(textureOffset(tex, uv, ivec2(1, 0)).rgb);
	float i22 = rgb_2_luma(textureOffset(tex, uv, ivec2(1, 1)).rgb);

	float gx = (-1.0 * i00) + (-2.0 * i01) + (-1.0 * i02) + (0.0 * i10) + (0.0 * i11) + (0.0 * i12) + (1.0 * i20) + (2.0 * i21) + (1.0 * i22);
	float gy = (-1.0 * i00) + (0.0 * i01) + (1.0 * i02) + (-2.0 * i10) + (0.0 * i11) + (2.0 * i12) + (-1.0 * i20) + (0.0 * i21) + (1.0 * i22);

	vec3 edge = vec3(1.0 - sqrt(gx * gx + gy * gy));
	color = edge;
#elif EFFECT_TYPE == 1
	/// TODO2: gaussian convolution goes here
	int size = 5;
	vec3 gaussColor = vec3(0.0);
	float sum_weights = 0;

	for (int i = -size; i < size; ++i) {
		for (int j = -size; j < size; ++j) {
			float weight = gauss(sqrt(float(i * i + j * j)));
			sum_weights += weight;
			gaussColor += (weight * texture(tex, uv + vec2(i / tex_width, j / tex_height)).rgb);
		}
	}

	gaussColor = gaussColor / sum_weights;
	color = gaussColor;
#elif EFFECT_TYPE == 2
	int size = 5;
	vec3 gaussColor = vec3(0.0);

	for (int i = -size; i < size; ++i) {
		gaussColor += (weights[i + 5] * texture(tex, uv + direction * i).rgb);
	}

	color = gaussColor;
#else
	color = texture(tex,uv).rgb;
#endif
}
