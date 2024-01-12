#version 450
out vec4 frag_colour;
in vec4 f_position;

void main () {

	// Set color proportional to the z position here, instead of a constant. 
	frag_colour = vec4((f_position.z - 0.3)*-1);
}
