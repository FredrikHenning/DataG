// Build on your code from Lab 1 here
#version 450

out vec3 frag_colour;
in vec3 f_position;
in vec3 normal;

void main () {
	frag_colour = (normal + 1.0) * 0.5;

	// frag_colour = vec4((f_position.z - 0.5)*-1);
}