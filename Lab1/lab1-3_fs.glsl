#version 400
out vec3 frag_colour;
in vec3 position;
uniform vec2 color_offset;

void main () {

	// Change this to use input from a `uniform` variable instead of just a constant color:
	
	// Change the colour with the scroll wheel
	//frag_colour = vec3(color_offset, 1.0) + vec3(0.5, 0.5, 0.5); 
	
	// Normal assignment
	frag_colour = position + vec3(0.5, 0.5, 0.5);

}
