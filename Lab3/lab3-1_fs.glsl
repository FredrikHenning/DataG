// Build on your code from Lab 1 here
#version 450

uniform sampler2D tex_sampler;

in vec3 f_position;
in vec3 normal;
in vec2 texture_coord;

out vec3 frag_colour;

vec4 text = texture(tex_sampler, texture_coord);

void main () {
	//frag_colour = (normal + 1.0) * 0.5;
	frag_colour = text.rgb;
}

// Note: Task 3.1 said to copy code from 2.1cpp but lightning is not
//		 implimented there. So i just set frag colour to text.rgb
//		 And brdf was not implimented untill task 2.2 also so i did not
//		 add that aswell. This will be added in task 3.2 instead