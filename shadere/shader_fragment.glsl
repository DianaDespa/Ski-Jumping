#version 330
layout(location = 0) out vec4 out_color;

uniform vec3 light_position;
uniform vec3 eye_position;
uniform int material_shininess;
uniform float material_kd;
uniform float material_ks;

in vec3 vertex_to_fragment_color;
in vec3 world_pos;
in vec3 world_normal;

void main() {
	float light = 0;
	float Ca = 0.0f;
	float Cd, Cs;

	vec3 L = normalize(light_position - world_pos),
		V = normalize(eye_position - world_pos),
		R = reflect(-L, world_normal),
		H = normalize(L + V);
	Cd = material_kd * max(dot(world_normal, L), 0);
	Cs = material_ks * pow(max(dot(world_normal, H), 0), material_shininess);

	light = Ca + Cd + Cs;
	out_color = vec4(vertex_to_fragment_color + vec3(light, light, light), 1);
}