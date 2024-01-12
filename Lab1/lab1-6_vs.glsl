#version 450

//layout(location=0) in vec4 vp;
in vec4 vp;
uniform mat4 position;
out vec4 f_position;

void main () {

	gl_Position =  position * vp;
	f_position = gl_Position;

};
