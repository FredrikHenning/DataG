#version 450

layout(location=0) in vec3 vp;
layout(location=1) in vec3 normals;

uniform mat4 mvpMatrix;
uniform mat4 model_view_matrix;

out vec3 f_position;
out vec3 f_normal;

vec4 vp4 = vec4(vp, 1.0f);
vec4 normals4 = vec4(normals, 1.0f);

void main () {

	gl_Position =  mvpMatrix * vp4;
	f_position = (model_view_matrix * vp4).xyz;
	f_normal = (model_view_matrix * normals4).xyz;

};
