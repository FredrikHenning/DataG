#version 450
out vec4 frag_colour;
in vec4 f_position;

void main () {

	frag_colour = vec4((f_position.z - 0.5)*-1);
}
