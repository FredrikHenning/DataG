#version 450

layout(location=0) in vec4 vp;
layout(location=1) in vec4 normals;
// in vec4 vp;

uniform mat4 mvpMatrix;
uniform mat4 model_view_matrix;

out vec3 f_position;
out vec3 f_normal;

void main () {

	gl_Position =  mvpMatrix * vp;
	f_position = (model_view_matrix * vp).xyz;
	f_normal = (model_view_matrix * normals).xyz;

};
