#version 330

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_color;
layout(location = 2) in vec3 in_normal;

uniform mat4 model_matrix, view_matrix, projection_matrix, scale_matrix;
uniform bool is_main;
uniform double time;
uniform float x_quake;
uniform float y_quake;
uniform float z_quake;

out vec3 vertex_to_fragment_color;
out vec3 world_pos;
out vec3 world_normal;

void main() {
	vec3 new_position;

	if (!is_main) {
		vertex_to_fragment_color = in_color;
	}
	else {
		double max_time = 20000;
		double n_time = time / max_time;
		vertex_to_fragment_color = vec3(1 - n_time, 0, 0) + vec3(0, 0, n_time);
	}

	if (x_quake > -1)
		new_position = vec3(x_quake, y_quake, z_quake) + in_position;
	else
		new_position = in_position;

	world_pos = (model_matrix * vec4(in_position, 1)).xyz;
	world_normal = normalize(mat3(model_matrix) * in_normal);
	gl_Position = projection_matrix * view_matrix * model_matrix * scale_matrix * vec4(new_position, 1); 
}
